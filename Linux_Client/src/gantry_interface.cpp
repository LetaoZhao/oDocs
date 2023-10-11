#include "gantry_interface.h"
#include "error_handler.h"
#include "gantry_config.h"  // TODO: Implement software bounding box
#include "video_processing.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
#include <cstring>

#include <condition_variable>

GantryInterface::GantryInterface() {
    // empty buffers
    memset(&_write_buf, '\0', sizeof(_write_buf));
    memset(&_read_buf, '\0', sizeof(_read_buf));

    _serial_port = open("/dev/ttyUSB0", O_RDWR);
    // Check for errors
    if (_serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        error_handler();
    }

    // Configure serial port
    _tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    _tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    _tty.c_cflag |= CS8; // 8 bits per byte (most common)
    _tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    _tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    _tty.c_lflag &= ~ICANON;
    _tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    _tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    _tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                      ICRNL); // Disable any special handling of received bytes
    _tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    _tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    // Save _tty settings, also checking for error
    if (tcsetattr(_serial_port, TCSANOW, &_tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        error_handler();
    }

    struct termios2 tio = {0};
    ioctl(_serial_port, TCGETS2, &tio);
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= CBAUDEX;
    tio.c_ispeed = 112500;
    tio.c_ospeed = 112500;
/* do other miscellaneous setup options with the flags here */
    if (ioctl(_serial_port, TCSETS2, &tio) != 0) {
        std::cout << "ERROR: couldn't initialize baud rate...";
        error_handler();
    };
    // Probably should be in a config file somewhere
    _commands.emplace_back("$100 = 200\n");     // Resolutions
    _commands.emplace_back("$101 = 200\n");
    _commands.emplace_back("$102 = 800\n");
    _commands.emplace_back("$110 = 2000\n");    // Max Speeds
    _commands.emplace_back("$111 = 2000\n");
    _commands.emplace_back("$112 = 2000\n");
    _commands.emplace_back("$120 = 2000\n");    // Max Accelerations
    _commands.emplace_back("$121 = 2000\n");
    _commands.emplace_back("$122 = 2000\n");

    _commands.emplace_back("$21 = 1\n");    // Enable Limits and Homing
    _commands.emplace_back("$22 = 1\n");
    _commands.emplace_back("$5 = 1\n");     // Invert limit switches
    _commands.emplace_back("$23 = 7\n");    // Invert homing directions

    _commands.emplace_back("$20 = 1\n");    // Enable Soft-Limits

    _commands.emplace_back("$130 = "+std::to_string(x_limit)+"\n"); // x, y, z bounding boxes
    _commands.emplace_back("$131 = "+std::to_string(y_limit)+"\n");
    _commands.emplace_back("$132 = "+std::to_string(z_limit)+"\n");

    _commands.emplace_back("$G\n");             // ???
    _commands.emplace_back("$X\n");
//    _commands.emplace_back("G21G90G0X"+std::to_string(x_limit-2)+"Y"+std::to_string(y_limit-2)+"Z"+std::to_string(z_limit-2)+"F2000\n");  // Align Top

    _commands.emplace_back("$H\n");             // Home Device
    _commands.emplace_back("G10 P0 L20 X0 Y0 Z0\n");    // Reset Device 0 to home
    _commands.emplace_back("G21G91G0X0Y0Z"+std::to_string(this->z_limit-2)+"F2000\n");  // Align Top
    while(!_newline_received());
}

// Read Write Functions
bool GantryInterface::_write_command(std::string const &command) {
    std::copy(command.begin(), command.end(), _write_buf);
    std::cout << _write_buf;
    int bytes_written = write(_serial_port, _write_buf, command.length());
    memset(&_write_buf, '\0', sizeof(_write_buf));
    if (bytes_written == -1) return false;
    if (bytes_written != command.length()) return false;
    return true;
};

bool GantryInterface::_newline_received() {
    bool newline = false;
    memset(&_read_buf, '\0', sizeof(_read_buf));
    int num_bytes = 0;
        while(num_bytes == 0) {
            num_bytes = read(_serial_port, &_read_buf, sizeof(_read_buf));
            if (num_bytes < 0) {
                std::cout << "Error reading: " << strerror(errno) << std::endl;
                newline = false;
            }
        }
        std::cout << _read_buf;
        for (int i = 0; i < num_bytes; i++) {
            if (_read_buf[i] == '\n') newline = true;
        }
    return newline;
}

// writing
// Will only function if output buffer is not full
// So we have to read the output buffer

// My favorite (w)rapper
void GantryInterface::move_to(int x, int y, int z, MOVE_MODE coord_system) {
    move_to(std::to_string(x),std::to_string(y),std::to_string(z), coord_system);
}

bool GantryInterface::_check_collision(int x, int y, int z) const {
    // Check rectangular mask
    if ((x > x_limit)||(x < 0)) return true;
    if ((y > y_limit)||(y < 0)) return true;
    if ((z > z_limit)||(z < 0)) return true;
    // Check cylindrical mask
    int radius = std::pow((float)(x - rail_x_offset),2)+std::pow((float)z,2);
    if ((radius > std::pow(armature_outer_radius,2))||(radius < std::pow(armature_inner_radius,2))) return true;
    return false;
}

void GantryInterface::move_to(std::string x, std::string y, std::string z, MOVE_MODE coord_system) {
    std::string command;
    if (coord_system == GANTRY_LOCAL) {
        command.append("G21G91G0X");
    } else {
        command.append("G21G90G0X");
    }
    int x_int = std::stoi(x);
    int y_int = std::stoi(y);
    int z_int = std::stoi(z);

    if (coord_system == GANTRY_LOCAL) {
        if (_check_collision(current_x + x_int,current_y + y_int,current_z + z_int > z_limit)) return;
    } else {
        if (_check_collision(x_int,y_int,z_int)) return;
    }

    command.append(x);
    command.append("Y");
    command.append(y);
    command.append("Z");
    command.append(z);
    command.append("F");
    command.append(_feed_rate);
    command.append("\n");
    std::lock_guard<std::mutex> guard(_command_queue_mutex);
    _commands.push_back(command);

    if (coord_system == GANTRY_LOCAL) {
        current_x += x_int;
        current_y += y_int;
        current_z += z_int;
    } else {
        current_x = x_int;
        current_y = y_int;
        current_z = z_int;
    }
    std::cout << "Gantry Pos:" << current_x << "," << current_y << "," << current_z << std::endl;
}

void GantryInterface::reset_gantry() {
    _commands.clear();
    _commands.emplace_back("$$\n");
    _commands.emplace_back("$X\n");
    _commands.emplace_back("G10 P0 L20 X0 Y0 Z0\n");    // Reset Device 0 to home
    _commands.emplace_back("G21G91G0X0Y0Z"+std::to_string(this->z_limit)+"F2000\n");  // Align Top
    _commands.emplace_back("G21G91G0X0Y0Z5F100\n");
}

// TODO: make directions consistent
void GantryInterface::process_message(const char *type, const char *message) {
    std::string type_string(type);
    std::string message_string(message);
    char delimiter = ',';
    if (type_string == "move_local") {
        // Fix delimiter finder
        int delimiter_1 = message_string.find(delimiter, 0);
        int delimiter_2 = message_string.find(delimiter, delimiter_1 + sizeof(char));

        std::string x_position = message_string.substr(0, delimiter_1);
        std::string y_position = message_string.substr(delimiter_1 + 1, delimiter_2 - delimiter_1 - 1);
        std::string z_position = message_string.substr(delimiter_2 + 1, message_string.length() - delimiter_2);

        move_to(x_position, y_position, z_position, GANTRY_LOCAL);

    } else if (type_string == "move_global") {
        // Fix delimiter finder
        int delimiter_1 = message_string.find(delimiter, 0);
        int delimiter_2 = message_string.find(delimiter, delimiter_1 + sizeof(char));

        std::string x_position = message_string.substr(0, delimiter_1);
        std::string y_position = message_string.substr(delimiter_1 + 1, delimiter_2 - delimiter_1 - 1);
        std::string z_position = message_string.substr(delimiter_2 + 1, message_string.length() - delimiter_2);

        move_to(x_position, y_position, z_position, GANTRY_GLOBAL);

    } else if (type_string == "home") {

        // Begin CV Eye Homing Sequence
        // LOGIC:
        // Move to approximate center of face
        // Get pointer to eyes vector
        // Move to that point
        // Get pointer to eyes vector
        move_to(x_home,y_home,z_home,GANTRY_GLOBAL);
        // Wait for the thing to stop moving
        wait_till_moving();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));    // Move must be at least 300ms long
        wait_till_stopped();

//        cv::Rect eye;
//        if (message_string == "left") {
//            eye = get_eye_pos(DIRECTION::LEFT); // gets eye pos from video thread
//        } else if(message_string == "right"){
//            eye = get_eye_pos(DIRECTION::RIGHT);
//        } else {
//            return;
//        }

        // Using inter-pupilliary distance
        if ((message_string == "right")||(message_string == "left")) {
//            int x_move = -(eye.x + eye.width/2-640/2)/3;
//            int y_move = -(eye.y + eye.height/2-480/2)/3;
            int y_move = 0;
            int x_move = message_string == "right" ? 32 : -32;

            move_to(x_move,0,y_move,GANTRY_LOCAL);
        }

    } else if (type_string == "config_step") {
        _feed_rate = message_string;
    } else if (type_string == "reset") {
        reset_gantry();
    } else if (type_string == "set_home") {
        x_home = current_x;
        y_home = current_y;
        z_home = current_z;
    }
}

bool GantryInterface::process_interface_io() {
    // Lock mutex while in scope
    std::lock_guard<std::mutex> guard(_command_queue_mutex);
    if (!_commands.empty()) {
        // if an ack was received, queue the current front command
        _write_command(_commands.front());
        _commands.pop_front();
        while (!_newline_received()) {}
    }
    return true;
}

