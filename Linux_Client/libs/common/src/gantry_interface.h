//
// Created by tazukiswift on 4/07/23.
//

#ifndef SIGNALING_GANTRY_INTERFACE_H
#define SIGNALING_GANTRY_INTERFACE_H

#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

void init_serial_port();

void process_message(const char *type, const char *message);

bool process_interface_io();

#endif //SIGNALING_GANTRY_INTERFACE_H
