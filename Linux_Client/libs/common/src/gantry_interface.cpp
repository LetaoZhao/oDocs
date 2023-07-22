#include "gantry_interface.h"

GantryInterface::GantryInterface() {
    // empty buffers
    memset(&_write_buf, '\0', sizeof(_write_buf));
    memset(&_read_buf, '\0', sizeof(_read_buf));

    _serial_port = open("/dev/ttyUSB0", O_RDWR);
    // Check for errors
    if (_serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    _tty = {0};

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
    };


    while (!_newline_received()) {}
    _write_command("$100 = 5.020\n");
    while (!_newline_received()) {}
    _write_command("$101 = 6.198\n");
    while (!_newline_received()) {}
    _write_command("$102 = 25.000\n");
    while (!_newline_received()) {}
}

// Read Write Functions
bool GantryInterface::_write_command(std::string const &command) {
    std::copy(command.begin(), command.end(), _write_buf);
    int bytes_written = write(_serial_port, _write_buf, command.length());
    memset(&_write_buf, '\0', sizeof(_write_buf));
    if (bytes_written == -1) return false;
    if (bytes_written != command.length()) return false;
    return true;
};

bool GantryInterface::_newline_received() {
    bool newline = false;
    memset(&_read_buf, '\0', sizeof(_read_buf));

    // TODO: rewrite so only clears readbuff when "ok" was received
    int num_bytes = read(_serial_port, &_read_buf, sizeof(_read_buf));

    for (int i = 0; i < num_bytes; i++) {
        if (_read_buf[i] == '\n') newline = true;
    }

    std::cout << _read_buf;
    if (num_bytes < 0) {
        std::cout << "Error reading: " << strerror(errno) << std::endl;
        newline = false;
    }
    return newline;
}

void GantryInterface::_init_serial_port() {

}

// writing
// Will only function if output buffer is not full
// So we have to read the output buffer

// TODO: strip G21G91 from this command, should be a config option
void GantryInterface::process_message(const char *type, const char *message) {
    std::string message_string(message);
    char delimiter = ',';

    // Fix delimiter finder
    int delimiter_1 = message_string.find(delimiter, 0);
    int delimiter_2 = message_string.find(delimiter, delimiter_1 + sizeof(char));

    std::string x_position = message_string.substr(0, delimiter_1);
    std::string y_position = message_string.substr(delimiter_1 + 1, delimiter_2 - delimiter_1 - 1);
    std::string z_position = message_string.substr(delimiter_2 + 1, message_string.length() - delimiter_2);
    std::string command;
    command.append("G21G91G0X");
    command.append(x_position);
    command.append("Y");
    command.append(y_position);
    command.append("Z");
    command.append(z_position);
    command.append("F100\n");

    std::lock_guard<std::mutex> guard(_command_queue_mutex);
    // Process message into command here
    _commands.push_back(command);
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
    // Process readbuffer
    return true;
}

