//
// Created by tazukiswift on 4/07/23.
//
#include "gantry_interface.h"
#include <mutex>
#include <string>

// sudo adduser $USER dialout if get error 13
int serial_port;
struct termios tty;

char read_buf[256];
char write_buf[32];

std::mutex command_queue_mutex;

// we need to read from the read_buf and print whatever characters there are
int buf_pointer = 0;

std::vector<std::string> commands;

void init_serial_port() {
    serial_port = open("/dev/ttyUSB0", O_RDWR);
    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    // Read in existing settings, and handle any error
    // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
    // must have been initialized with a call to tcgetattr() overwise behaviour
    // is undefined
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    // Configure serial port
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                     ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    int ispeed_error = cfsetispeed(&tty, 112500);
    int ospeed_error = cfsetospeed(&tty, 112500);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    // empty buffers
    memset(&write_buf, '\0', sizeof(write_buf));
    memset(&read_buf, '\0', sizeof(read_buf));
}


// writing
// Will only function if output buffer is not full
// So we have to read the output buffer

void process_message(const char *message) {
    std::string message_string(message);
    char delimiter = ',';

    // Fix delimiter finder
    std::string x_position = message_string.substr(0, message_string.find(delimiter));
    std::string y_position = message_string.substr(0, message_string.find(delimiter));

    std::string command = "";
    command.append("G21G91G0X");
    command.append(x_position);
    command.append("Y");
    command.append(y_position);
    command.append("F100\n");

    std::lock_guard<std::mutex> guard(command_queue_mutex);
    // Process message into command here
    commands.emplace_back(command);
}


bool process_interface_io() {
    // Lock mutex while in scope
    std::lock_guard<std::mutex> guard(command_queue_mutex);
    if (((read_buf[buf_pointer] == '\n') && (!commands.empty()))) {
        std::copy(commands.front().begin(), commands.front().end(), write_buf);
        write(serial_port, write_buf, commands.front().length());
        commands.erase(commands.begin());
        memset(&write_buf, '\0', sizeof(write_buf));
    }
    memset(&read_buf, '\0', sizeof(read_buf));
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
    buf_pointer = num_bytes - 1;
    std::cout << read_buf;
    if (num_bytes < 0) {
        std::cout << "Error reading: " << strerror(errno) << std::endl;
    }
    return true;
}

