# Documentation of C++ Client

---

The purpose of the C++ Client is to provide a bridge between opentok and the telemedicine gantry. It publishes a camera
stream to an opentok session and allows for control via a protocol built on opentok signaling.

## Hardware Setup

---

This software interfaces with any printer running `GRBL` via usb-serial at a baudrate of 115200 bit/s. Programming the
bounding box is required as this software uses soft limits, this should be done in the `gantry_config.h` header. With
this in place the device will work.

Additionally, a USB-WEBCAM currently needs to be plugged in to publish the video stream back to opentok.

## Using the Client (quick-start)

This client is standalone, and the built executable is meant to be called externally. `The API-KEY`, `SESSION_ID`, and  
`TOKEN` are passed as arguments, shown below for calling the code from bash.

```bash
./odocs_gantry --key <key> --id <id> --token <token>
```

Provided that the gantry and web-cam are plugged in, it will then start, home and receive commands

## Control Protocol

The gantry currently receives commands via opentok signaling protocol. Provided a valid api-key, session-id and
token are provided it will receive commands in the format-spec outlined below:

| Signal Type   | Signal Contents Formatspec | Description                           |
|:--------------|:---------------------------|---------------------------------------|
| `move_local`  | `<dx>,<dy>,<dz>`           | Moves relative to current position    |
| `move_global` | `<x>,<y>,<z>`              | Moves to global position              |
| `home`        | `<eye>`                    | Moves to either `left` or `right` eye |
| `config_step` | `<feed_rate>`              | Changes the speed of the gantry       |

## Code Breakdown

The code is well commented, so should be reviewed by itself for functional description.  However, a high level 
breakdown is provided here, with the aim to assist someone with digesting the application.

 ```
├── include
    ├── error_handler.h
    ├── gantry_config.h
    ├── gantry_interface.h
    ├── otk_thread.h
    ├── renderer.h
├── src
    ├── error_handler.cpp       
    ├── gantry_interface.cpp
    ├── main.cpp
    ├── otk_thread.c
    ├── renderer.cpp
 ```
 
The gantry itself contains an api for control explained below. 

### `gantry_config.h`
Defines the physical properties of the gantry

### `gantry_interface.h`

`GantryInterface()` constructs the gantry.  Uses default `tty_USB0` object in `/dev` to do so.

`void process_message(const char *type, const char *message)` Called to process a received opentok signal defined by 
the protocol above

`void move_to(int x, int y, int z, MOVE_MODE coord_system)` Moves to a position in `mm`, specifying coordinate 
system AS `GANTRY_LOCAL` or `GANTRY_GLOBAL` for local vs global moves.

`void move_to(std::string x, std::string y, std::string z, MOVE_MODE coord_system)` Overloads above function to take 
strings, useful for processing opentok messages.

`bool process_interface_io()` Tick function needs to be called regularly in code for gantry to process polling based 
serial functions.