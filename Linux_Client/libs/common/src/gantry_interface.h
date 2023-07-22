//
// Created by tazukiswift on 4/07/23.
//

#ifndef SIGNALING_GANTRY_INTERFACE_H
#define SIGNALING_GANTRY_INTERFACE_H

#include <iostream>
#include <cstring>
#include <string>
#include <chrono>
#include <mutex>
#include <string>
#include <deque>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <cerrno> // Error integer and strerror() function

#include <sys/ioctl.h>

//#include <asm/termios.h> // Contains POSIX terminal control definitions
#define termios asmtermios

#include <asm/termbits.h>
#include <asm/ioctls.h>

#undef termios

#include <termios.h>
#include <unistd.h> // write(), read(), close()

class GantryInterface {
private:
    void _init_serial_port();

    bool _newline_received();

    bool _write_command(std::string const &command);

private:
    // sudo adduser $USER dialout if get error 13
    int _serial_port;
    struct termios _tty;

    char _read_buf[256];
    char _write_buf[256];

    std::mutex _command_queue_mutex;

// TODO: replace this with a deque
    std::deque<std::string> _commands;

public:
    GantryInterface();

    void process_message(const char *type, const char *message);

    bool process_interface_io();
};

#endif //SIGNALING_GANTRY_INTERFACE_H
