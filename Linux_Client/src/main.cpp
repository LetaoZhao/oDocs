#include <atomic>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>

#include "renderer.h"
#include "gantry_interface.h"
#include <video_processing.h>

#include <opentok.h>

static std::atomic<bool> g_is_connected(false);
static otc_publisher *g_publisher = nullptr;
static std::atomic<bool> g_is_publishing(false);
static std::map<std::string, void*> g_subscriber_map;
static std::mutex g_subscriber_map_mutex;

// Camera USB Device
std::string camera_usb_device;

// Don't tell anyone but...
GantryInterface* gantry;


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
    gantry->process_message(type, signal);
    std::cout << "Type: " << type << ", Signal: " << signal << std::endl;
}

static void on_subscriber_connected(otc_subscriber *subscriber,
                                    void *user_data,
                                    const otc_stream *stream) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_subscriber_render_frame(otc_subscriber *subscriber,
                                       void *user_data,
                                       const otc_video_frame *frame) {
    RendererManager *render_manager = static_cast<RendererManager*>(user_data);
    if (render_manager == nullptr) {
        return;
    }
    render_manager->addFrame(subscriber, frame);
}

static void on_subscriber_error(otc_subscriber* subscriber,
                                void *user_data,
                                const char* error_string,
                                enum otc_subscriber_error_code error) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    std::cout << "Subscriber error. Error code: " << error_string << std::endl;
}

static void on_session_connected(otc_session *session, void *user_data) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;

    g_is_connected = true;

    if ((session != nullptr) && (g_publisher != nullptr)) {
        if (otc_session_publish(session, g_publisher) == OTC_SUCCESS) {
            g_is_publishing = true;
            return;
        }
        std::cout << "Could not publish successfully" << std::endl;
    }
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
    RendererManager *render_manager = static_cast<RendererManager*>(user_data);
    if (render_manager == nullptr) {
        return;
    }

    struct otc_subscriber_callbacks subscriber_callbacks = {0};
    subscriber_callbacks.user_data = user_data;
    subscriber_callbacks.on_connected = on_subscriber_connected;
    subscriber_callbacks.on_render_frame = on_subscriber_render_frame;
    subscriber_callbacks.on_error = on_subscriber_error;

    otc_subscriber *subscriber = otc_subscriber_new(stream,
                                                    &subscriber_callbacks);
    if (subscriber == nullptr) {
        std::cout << "Could not create OpenTok subscriber successfully" << std::endl;
        return;
    }
    render_manager->createRenderer(subscriber);
    if (otc_session_subscribe(session, subscriber) == OTC_SUCCESS) {
        const std::lock_guard<std::mutex> lock(g_subscriber_map_mutex);
        g_subscriber_map[std::string(otc_stream_get_id(stream))] = subscriber;
        return;
    }
    std::cout << "Could not subscribe successfully" << std::endl;
}

static void on_session_stream_dropped(otc_session *session,
                                      void *user_data,
                                      const otc_stream *stream) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    otc_subscriber *subscriber = static_cast<otc_subscriber *>(g_subscriber_map[std::string(otc_stream_get_id(stream))]);
    if (subscriber == nullptr) {
        return;
    }

    RendererManager *render_manager = static_cast<RendererManager*>(user_data);
    if (render_manager != nullptr) {
        render_manager->destroyRenderer(subscriber);
    }
    otc_subscriber_delete(subscriber);
    const std::lock_guard<std::mutex> lock(g_subscriber_map_mutex);
    g_subscriber_map[std::string(otc_stream_get_id(stream))] = nullptr;
    subscriber = nullptr;
}

static void on_session_disconnected(otc_session *session, void *user_data) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_session_error(otc_session *session,
                             void *user_data,
                             const char *error_string,
                             enum otc_session_error_code error) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    std::cout << "Session error. Error : " << error_string << std::endl;
}

static void on_publisher_stream_created(otc_publisher *publisher,
                                        void *user_data,
                                        const otc_stream *stream) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_publisher_render_frame(otc_publisher *publisher,
                                      void *user_data,
                                      const otc_video_frame *frame) {
    RendererManager *render_manager = static_cast<RendererManager*>(user_data);
    if (render_manager == nullptr) {
        return;
    }
    render_manager->addFrame(publisher, frame);
}

static void on_publisher_stream_destroyed(otc_publisher *publisher,
                                          void *user_data,
                                          const otc_stream *stream) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
}

static void on_publisher_error(otc_publisher *publisher,
                               void *user_data,
                               const char* error_string,
                               enum otc_publisher_error_code error_code) {
    std::cout << __FUNCTION__ << " callback function" << std::endl;
    std::cout << "Publisher error. Error code: " << error_string << std::endl;
}

static void on_otc_log_message(const char* message) {
    std::cout <<  __FUNCTION__ << ":" << message << std::endl;
}

// Cursed
RendererManager renderer_manager;

void system_loop_wrapper() {
    gantry = new GantryInterface();
    while (gantry->process_interface_io()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    };
}

void event_loop_wrapper() {
    renderer_manager.runEventLoop();
}

// Handle Arguments
bool handle_args(int argc, char *argv[], std::string *API_KEY, std::string *SESSION_ID, std::string *TOKEN) {
    int flag = 0;
    for (int i = 0;i<argc;i++) {
        if (strcmp(argv[i],"--key") == 0) {
            *API_KEY = argv[i+1];
            i++;
            flag++;
        }
        if (strcmp(argv[i],"--id") == 0) {
            *SESSION_ID = argv[i+1];
            i++;
            flag++;
        }
        if (strcmp(argv[i],"--token") == 0) {
            *TOKEN = argv[i+1];
            i++;
            flag++;
        }
        if (strcmp(argv[i],"--device") == 0) {
            camera_usb_device = argv[i+1];
            i++;
            flag++;
        }
    }
    if (flag!=4) return false;
    std::cout << "API KEY = " << *API_KEY << std::endl;
    std::cout << "SESSION ID = " << *SESSION_ID << std::endl;
    std::cout << "TOKEN = " << *TOKEN << std::endl;
    std::cout << "DEVICE = " << camera_usb_device << std::endl;
    return true;
}

// Call with api-key, session id, session token
int main(int argc, char *argv[]) {
    if (otc_init(nullptr) != OTC_SUCCESS) {
        std::cout << "Could not init OpenTok library" << std::endl;
        return EXIT_FAILURE;
    }

    std::string API_KEY;
    std::string SESSION_ID;
    std::string TOKEN;

    if (!handle_args(argc,argv,&API_KEY,&SESSION_ID,&TOKEN)) {
        std::cout << "API_KEY, SESSION_ID or TOKEN not found" << std::endl;
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

    // Publisher
    struct otc_publisher_callbacks publisher_callbacks = {0};
    publisher_callbacks.user_data = &renderer_manager;
    publisher_callbacks.on_stream_created = on_publisher_stream_created;
    publisher_callbacks.on_render_frame = on_publisher_render_frame;
    publisher_callbacks.on_stream_destroyed = on_publisher_stream_destroyed;
    publisher_callbacks.on_error = on_publisher_error;

    // Custom Video Capturer
    auto *video_capturer = (struct custom_video_capturer *)malloc(sizeof(struct custom_video_capturer));
    video_capturer->video_capturer_callbacks = {0};
    video_capturer->video_capturer_callbacks.user_data = static_cast<void *>(video_capturer);
    video_capturer->video_capturer_callbacks.init = video_capturer_init;
    video_capturer->video_capturer_callbacks.destroy = video_capturer_destroy;
    video_capturer->video_capturer_callbacks.start = video_capturer_start;
    video_capturer->video_capturer_callbacks.get_capture_settings = get_video_capturer_capture_settings;
    video_capturer->width = 640;
    video_capturer->height = 480;

    otc_session *session = nullptr;

    session = otc_session_new(API_KEY.c_str(), SESSION_ID.c_str(), &session_callbacks);

    if (session == nullptr) {
        std::cout << "Could not create OpenTok session successfully" << std::endl;
        return EXIT_FAILURE;
    }

    // Register Publisher
    g_publisher = otc_publisher_new("opentok-linux-sdk-samples",
//                                    &(video_capturer->video_capturer_callbacks),
                                    &video_capturer->video_capturer_callbacks,
                                    &publisher_callbacks);
    if (g_publisher == nullptr) {
        std::cout << "Could not create OpenTok publisher successfully" << std::endl;
        otc_session_delete(session);
        return EXIT_FAILURE;
    }

    renderer_manager.createRenderer(g_publisher);

    otc_session_connect(session, TOKEN.c_str());

    std::thread system_loop(system_loop_wrapper);

    std::thread event_loop(event_loop_wrapper);

    event_loop.join();

    renderer_manager.destroyRenderer(g_publisher);

    if ((session != nullptr) && (g_publisher != nullptr) && g_is_publishing.load()) {
        otc_session_unpublish(session, g_publisher);
    }

    if (g_publisher != nullptr) {
        otc_publisher_delete(g_publisher);
        g_publisher = nullptr;
    }

    if ((session != nullptr) && g_is_connected.load()) {
        otc_session_disconnect(session);
    }

    if (session != nullptr) {
        otc_session_delete(session);
    }

    otc_destroy();

    return EXIT_SUCCESS;
}
