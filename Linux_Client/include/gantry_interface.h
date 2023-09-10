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

    bool _newline_received();

    bool _write_command(std::string const &command);

private:
    // sudo adduser $USER dialout if get error 13
    int _serial_port;
    struct termios _tty = {0};  // NGL this kinda scuffed

    char _read_buf[256] = {0};
    char _write_buf[256] = {0};

    std::string _feed_rate = "2000";

    std::mutex _command_queue_mutex;

    std::deque<std::string> _commands;
public:
    enum MOVE_MODE {
        GANTRY_LOCAL,
        GANTRY_GLOBAL
    };
public:
    GantryInterface();

    void process_message(const char *type, const char *message);

    void move_to(int x, int y, int z, MOVE_MODE coord_system);

    void move_to(std::string x, std::string y, std::string z, MOVE_MODE coord_system);

    bool process_interface_io();
};

#endif //SIGNALING_GANTRY_INTERFACE_H
