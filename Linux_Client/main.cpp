#include <opentok.h>

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "config.h"
#include "renderer.h"
#include "gantry_interface.h"

static std::atomic<bool> g_is_connected(false);


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

    if (session == nullptr) {
        return;
    }
    process_message(signal);
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

void event_loop_wrapper() {
    renderer_manager.runEventLoop();
}

void system_loop_wrapper() {
    while (process_interface_io()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };
}

int main(int argc, char **argv) {
    if (otc_init(nullptr) != OTC_SUCCESS) {
        std::cout << "Could not init OpenTok library" << std::endl;
        return EXIT_FAILURE;
    }

    // Begin Serial Setup
    init_serial_port();
    char *command_1 = "1,0";
    char *command_2 = "-1,0";
    process_message(command_1);
    process_message(command_2);
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

    session = otc_session_new(API_KEY, SESSION_ID, &session_callbacks);

    if (session == nullptr) {
        std::cout << "Could not create OpenTok session successfully" << std::endl;
        return EXIT_FAILURE;
    }

    otc_session_connect(session, TOKEN);

//  renderer_manager.runEventLoop();
    std::thread event_loop(event_loop_wrapper);

    std::thread system_loop(system_loop_wrapper);

    if ((session != nullptr) && g_is_connected.load()) {
        otc_session_disconnect(session);
    }

    if (session != nullptr) {
        otc_session_delete(session);
    }

    event_loop.join();


    otc_destroy();

    return EXIT_SUCCESS;
}
