
#include "video_processing.h"
#include "renderer.h"

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include <atomic>

#include <condition_variable>

extern std::string camera_usb_device;

std::condition_variable stopped_condition;
std::mutex stopped_mutex;
bool stopped = false;

// Image Detection stuff (move somewhere else)
cv::CascadeClassifier eyes_cascade;

// vector of all eyes in frame
std::vector<cv::Rect> eyes;

// mutex for using the current frame
std::mutex frame_mutex;                                     // Mutex
std::atomic<bool> writing_frame_zero(true);
cv::Mat frame_buffer_zero = cv::Mat(480, 640, CV_8UC4);   // The Frame Itself
cv::Mat frame_buffer_one = cv::Mat(480, 640, CV_8UC4);   // The Frame Itself
cv::Mat frame_buffer_raw = cv::Mat(640, 480, CV_8UC4);   // The Frame Itself

otc_bool video_capturer_init(const otc_video_capturer *capturer, void *user_data) {
    struct custom_video_capturer *video_capturer = static_cast<struct custom_video_capturer *>(user_data);
    if (video_capturer == nullptr) {
        return OTC_FALSE;
    }
    video_capturer->width = 640;
    video_capturer->height = 480;
    video_capturer->video_capturer = capturer;

    return OTC_TRUE;
}

otc_bool video_capturer_destroy(const otc_video_capturer *capturer, void *user_data) {
    auto *video_capturer = static_cast<struct custom_video_capturer *>(user_data);
    if (video_capturer == nullptr) {
        return OTC_FALSE;
    }

    video_capturer->capturer_thread_exit = true;
    otk_thread_join(video_capturer->capturer_thread);

    return OTC_TRUE;
}

otc_bool video_capturer_start(const otc_video_capturer *capturer, void *user_data) {
    auto *video_capturer = static_cast<struct custom_video_capturer *>(user_data);
    if (video_capturer == nullptr) {
        return OTC_FALSE;
    }

    video_capturer->capturer_thread_exit = false;
    if (otk_thread_create(&(video_capturer->capturer_thread), &capturer_thread_start_function, (void *)video_capturer) != 0) {
        return OTC_FALSE;
    }

    return OTC_TRUE;
}

otc_bool get_video_capturer_capture_settings(const otc_video_capturer *capturer,
                                                    void *user_data,
                                                    struct otc_video_capturer_settings *settings) {
    struct custom_video_capturer *video_capturer = static_cast<struct custom_video_capturer *>(user_data);
    if (video_capturer == nullptr) {
        return OTC_FALSE;
    }

    settings->format = OTC_VIDEO_FRAME_FORMAT_ARGB32;
    settings->width = video_capturer->width;
    settings->height = video_capturer->height;
    settings->fps = 30;
    settings->mirror_on_local_render = OTC_FALSE;
    settings->expected_delay = 0;

    return OTC_TRUE;
}

// TODO: add this as an argument to program
otk_thread_func_return_type capturer_thread_start_function(void *arg) {
//    cv::VideoCapture vcap = cv::VideoCapture("/dev/video2");
    cv::VideoCapture vcap = cv::VideoCapture(camera_usb_device);


    struct custom_video_capturer *video_capturer = static_cast<struct custom_video_capturer *>(arg);
    if (video_capturer == nullptr) {
        otk_thread_func_return_value;
    }

    // Make sure eye_cascade is loaded
    eyes_cascade.load("../libraries/haar_cascades/haarcascade_eye.xml");

    const uint8_t* buffer;
    // We want this to get a request for eye position
    while(!video_capturer->capturer_thread_exit.load()) {
        // Lock frame for redraw
        frame_mutex.lock();
        cv::Mat * frame_buffer = writing_frame_zero ? &frame_buffer_zero : &frame_buffer_one;
        writing_frame_zero = !writing_frame_zero;
        vcap.read(frame_buffer_raw);
        cv::rotate(frame_buffer_raw,*frame_buffer,cv::ROTATE_90_COUNTERCLOCKWISE);
//        cv::imshow("CV Rotated",*frame_buffer);
//        cv::waitKey(1);
        buffer = (uint8_t *)(frame_buffer->datastart);
        otc_video_frame *otc_frame = otc_video_frame_new(OTC_VIDEO_FRAME_FORMAT_RGB24, video_capturer->height, video_capturer->width, buffer);
        otc_video_capturer_provide_frame(video_capturer->video_capturer, 0, otc_frame);
        frame_mutex.unlock();

        if (otc_frame != nullptr) {
            otc_video_frame_delete(otc_frame);
        }
        usleep(1000 / 30 * 1000);
    }
    otk_thread_func_return_value;
}

cv::Rect get_eye_pos(DIRECTION which_eye) {

    // By using double buffering, we can minimize the time that the functions block each other
    frame_mutex.lock();
    cv::Mat * frame_buffer = !writing_frame_zero ? &frame_buffer_zero : &frame_buffer_one;
    eyes_cascade.detectMultiScale(*frame_buffer, eyes, 1.1, 40, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(70, 70));

    if (!eyes.empty()) {
        for (auto &eye: eyes) {
            cv::rectangle(*frame_buffer, eye, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
        }
    }
    frame_mutex.unlock();

    cv::Rect the_eye;
    if (which_eye == DIRECTION::LEFT) {
        int eye_xpos = 1000;
        if (!eyes.empty()) {
            for (auto &eye: eyes) {
                if (eye.x < eye_xpos) {
                    eye_xpos = eye.x;
                    the_eye = eye;
                }
            }
        }
    } else {
        int eye_xpos = 0;
        if (!eyes.empty()) {
            for (auto &eye: eyes) {
                if (eye.x > eye_xpos) {
                    eye_xpos = eye.x;
                    the_eye = eye;
                }
            }
        }
    }
    return the_eye;
}

void wait_till_stopped() {
    cv::Mat prev;
    cv::Mat next;
    cv::Mat diff;

    int motion_pixels = 210001;
    while(motion_pixels > 209999) {
        frame_mutex.lock();
        cv::cvtColor(frame_buffer_zero, prev, cv::COLOR_BGR2GRAY);
        cv::cvtColor(frame_buffer_one, next, cv::COLOR_BGR2GRAY);
        frame_mutex.unlock();

        cv::absdiff(prev, next, diff);
        motion_pixels = cv::countNonZero(diff);
        std::cout << "Motion Pixels:" << motion_pixels << std::endl;
    }
}

void wait_till_moving() {
    cv::Mat prev;
    cv::Mat next;
    cv::Mat diff;

    int motion_pixels = 209999;
    while(motion_pixels < 210000) {
        frame_mutex.lock();
        cv::cvtColor(frame_buffer_zero, prev, cv::COLOR_BGR2GRAY);
        cv::cvtColor(frame_buffer_one, next, cv::COLOR_BGR2GRAY);
        frame_mutex.unlock();

        cv::absdiff(prev, next, diff);
        motion_pixels = cv::countNonZero(diff);
        std::cout << "Motion Pixels:" << motion_pixels << std::endl;
    }
}