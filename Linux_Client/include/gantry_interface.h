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

    bool _check_collision(int x, int y, int z) const;

    bool _newline_received();

    bool _write_command(std::string const &command);

private:
    // sudo adduser $USER dialout if get error 13
    int _serial_port;
    struct termios _tty = {0};  // NGL this kinda scuffed

    char _read_buf[256] = {0};
    char _write_buf[256] = {0};

    int rail_x_offset = 103;
    int armature_inner_radius = 100;
    int armature_outer_radius = 200;

    static const int x_limit = 200;
    static const int y_limit = 51;
    static const int z_limit = 110;

    // gantry config (messy solution)
    int x_home = rail_x_offset;
    int y_home = 0;
    int z_home = z_limit/2;

    int current_x = 0;
    int current_y = 0;
    int current_z = z_limit-2;

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

    void reset_gantry();
};

#endif //SIGNALING_GANTRY_INTERFACE_H
