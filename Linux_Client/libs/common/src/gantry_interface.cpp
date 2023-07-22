//
// Created by tazukiswift on 4/07/23.
//
#include "gantry_interface.h"
#include <mutex>
#include <string>
#include <deque>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

#include <sys/ioctl.h>

//#include <asm/termios.h> // Contains POSIX terminal control definitions
#define termios asmtermios

#include <asm/termbits.h>
#include <asm/ioctls.h>

#undef termios

#include <termios.h>
#include <unistd.h> // write(), read(), close()


// sudo adduser $USER dialout if get error 13
int serial_port;
struct termios tty;

char read_buf[256];
char write_buf[256];

std::mutex command_queue_mutex;

// TODO: replace this with a deque
std::deque<std::string> commands;

// Read Write Functions
bool write_command(std::string const &command) {
    std::copy(command.begin(), command.end(), write_buf);
    int bytes_written = write(serial_port, write_buf, command.length());
    memset(&write_buf, '\0', sizeof(write_buf));
    if (bytes_written == -1) return false;
    if (bytes_written != command.length()) return false;
    return true;
};

bool newline_received() {
    bool newline = false;
    memset(&read_buf, '\0', sizeof(read_buf));

    // TODO: rewrite so only clears readbuff when "ok" was received
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    for (int i = 0; i < num_bytes; i++) {
        if (read_buf[i] == '\n') newline = true;
    }

    std::cout << read_buf;
    if (num_bytes < 0) {
        std::cout << "Error reading: " << strerror(errno) << std::endl;
        newline = false;
    }
    return newline;
}

void init_serial_port() {
    serial_port = open("/dev/ttyUSB0", O_RDWR);
    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
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

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    struct termios2 tio;
    ioctl(serial_port, TCGETS2, &tio);
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= CBAUDEX;
    tio.c_ispeed = 112500;
    tio.c_ospeed = 112500;
/* do other miscellaneous setup options with the flags here */
    int error = ioctl(serial_port, TCSETS2, &tio);

    // empty buffers
    memset(&write_buf, '\0', sizeof(write_buf));
    memset(&read_buf, '\0', sizeof(read_buf));

    while (!newline_received()) {}
    write_command("$100 = 5.020\n");
    while (!newline_received()) {}
    write_command("$101 = 6.198\n");
    while (!newline_received()) {}
    write_command("$102 = 25.000\n");
    while (!newline_received()) {}
}

// writing
// Will only function if output buffer is not full
// So we have to read the output buffer

// TODO: strip G21G91 from this command, should be a config option
void process_message(const char *type, const char *message) {
    std::string message_string(message);
    char delimiter = ',';

    // Fix delimiter finder
    int delimiter_1 = message_string.find(delimiter, 0);
    int delimiter_2 = message_string.find(delimiter, delimiter_1 + sizeof(char));

    std::string x_position = message_string.substr(0, delimiter_1);
    std::string y_position = message_string.substr(delimiter_1 + 1, delimiter_2 - delimiter_1 - 1);
    std::string z_position = message_string.substr(delimiter_2 + 1, message_string.length() - delimiter_2);
    std::string command = "";
    command.append("G21G91G0X");
    command.append(x_position);
    command.append("Y");
    command.append(y_position);
    command.append("Z");
    command.append(z_position);
    command.append("F100\n");

    std::lock_guard<std::mutex> guard(command_queue_mutex);
    // Process message into command here
    commands.push_back(command);
}

bool process_interface_io() {
    // Lock mutex while in scope
    std::lock_guard<std::mutex> guard(command_queue_mutex);
    if (!commands.empty()) {
        // if an ack was received, queue the current front command
        write_command(commands.front());
        commands.pop_front();
        while (!newline_received()) {}
    }
    // Process readbuffer
    return true;
}

