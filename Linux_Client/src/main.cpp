#include <opentok.h>

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>

#include "renderer.h"
#include "gantry_interface.h"

static std::atomic<bool> g_is_connected(false);

// Don't tell anyone but...
GantryInterface gantry;

static void on_session_connected(otc_session *session, void *user_data) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    g_is_connected = true;
}

static void on_session_connection_created(otc_session *session,
                                          void *user_data,
                                          const otc_connection *connection) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_session_connection_dropped(otc_session *session,
                                          void *user_data,
                                          const otc_connection *connection) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_session_stream_received(otc_session *session,
                                       void *user_data,
                                       const otc_stream *stream) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_session_signal_received(otc_session *session,
                                       void *user_data,
                                       const char *type,
                                       const char *signal,
                                       const otc_connection *connection) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    std::cout << "RECEIVED A SIGNAL" << std::endl;
    if (session == nullptr) {
        return;
    }
    gantry.process_message(type, signal);
    std::cout << "Type: " << type << ", Signal: " << signal << std::endl;
}

static void on_session_stream_dropped(otc_session *session,
                                      void *user_data,
                                      const otc_stream *stream) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_session_disconnected(otc_session *session, void *user_data) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_session_error(otc_session *session,
                             void *user_data,
                             const char *error_string,
                             enum otc_session_error_code error) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    std::cout << "Session error. Error is: " << error_string << std::endl;
}

static void on_otc_log_message(const char *message) {
    std::cout << __FUNCTION__ << ":" << message << std::endl;
}

// Cursed
RendererManager renderer_manager;

void system_loop_wrapper() {
    while (gantry.process_interface_io()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };
}

void event_loop_wrapper() {
    renderer_manager.runEventLoop();
}

int main(int argc, char **argv) {
    if (otc_init(nullptr) != OTC_SUCCESS) {
        std::cout << "Could not init OpenTok library" << std::endl;
        return EXIT_FAILURE;
    }

#ifdef CONSOLE_LOGGING
    otc_log_set_logger_callback(on_otc_log_message);
    otc_log_enable(OTC_LOG_LEVEL_ALL);
#endif
    // Register callbacks
    struct otc_session_callbacks session_callbacks = {0};
    session_callbacks.on_connected = on_session_connected;
    session_callbacks.on_connection_created = on_session_connection_created;
    session_callbacks.on_connection_dropped = on_session_connection_dropped;
    session_callbacks.on_stream_received = on_session_stream_received;
    // This sample listens for signals.
    session_callbacks.on_signal_received = on_session_signal_received;
    session_callbacks.on_stream_dropped = on_session_stream_dropped;
    session_callbacks.on_disconnected = on_session_disconnected;
    session_callbacks.on_error = on_session_error;

    otc_session *session = nullptr;
    std::fstream config_file;
    config_file.open("../session.cfg", std::ios::in);
    std::string API_KEY;
    std::string SESSION_ID;
    std::string TOKEN;

    if (config_file.is_open()) {
        getline(config_file, API_KEY);
        getline(config_file, SESSION_ID);
        getline(config_file, TOKEN);
    } else {
        return EXIT_FAILURE;
    }

    session = otc_session_new(API_KEY.c_str(), SESSION_ID.c_str(), &session_callbacks);

    if (session == nullptr) {
        std::cout << "Could not create OpenTok session successfully" << std::endl;
        return EXIT_FAILURE;
    }

    otc_session_connect(session, TOKEN.c_str());

    std::thread system_loop(system_loop_wrapper);

    std::thread event_loop(event_loop_wrapper);

    event_loop.join();

    if ((session != nullptr) && g_is_connected.load()) {
        otc_session_disconnect(session);
    }

    if (session != nullptr) {
        otc_session_delete(session);
    }


    otc_destroy();

    return EXIT_SUCCESS;
}
