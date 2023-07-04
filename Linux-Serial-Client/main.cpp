#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


int main() {
    // sudo adduser $USER dialout if get error 13
    int serial_port = open("/dev/ttyUSB0", O_RDWR);

    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    struct termios tty;
    // Read in existing settings, and handle any error
    // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
    // must have been initialized with a call to tcgetattr() overwise behaviour
    // is undefined
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    int ispeed_error = cfsetispeed(&tty, 112500);
    int ospeed_error = cfsetospeed(&tty, 112500);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    // writing
    // Will only function if output buffer is not full
    // So we have to read the output buffer

    std::vector<std::string> commands;
    commands.emplace_back("G21G91G1Y1F100\n");
    commands.emplace_back("G21G91G1X1F100\n");
    commands.emplace_back("G21G91G1Y-1F100\n");
    commands.emplace_back("G21G91G1X-1F100\n");
    commands.emplace_back("G21G91G1Z1F100\n");
    commands.emplace_back("G21G91G1Z-1F100\n");

    char read_buf [256];
    char write_buf[32];

    memset(&write_buf, '\0', sizeof(write_buf));
    memset(&read_buf, '\0', sizeof(read_buf));

    // flag for waiting after wakeup message
    bool first_loop_flag = true;

    // we need to read from the read_buf and print whatever characters there are
    int buf_pointer = 0;
    while(1) {
        if (((read_buf[buf_pointer] == '\n')&&(!commands.empty()))) {
            std::copy( commands.front().begin(), commands.front().end(), write_buf);
            write(serial_port,write_buf,commands.front().length());
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
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    std::cout << "End of commands " << std::endl;
    close(serial_port);
    return 0;
}
