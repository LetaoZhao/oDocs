#ifndef ODOCS_GANTRY_VIDEO_PROCESSING_H
#define ODOCS_GANTRY_VIDEO_PROCESSING_H

#include <opentok.h>
#include "otk_thread.h"
#include <atomic>
#include <opencv2/opencv.hpp>

// Custom Video Interpreter
struct custom_video_capturer {
    const otc_video_capturer *video_capturer;
    struct otc_video_capturer_callbacks video_capturer_callbacks;
    int width;
    int height;
    otk_thread_t capturer_thread;
    std::atomic<bool> capturer_thread_exit;
};

otk_thread_func_return_type capturer_thread_start_function(void *arg);

otc_bool video_capturer_init(const otc_video_capturer *capturer, void *user_data);

otc_bool video_capturer_destroy(const otc_video_capturer *capturer, void *user_data);

otc_bool video_capturer_start(const otc_video_capturer *capturer, void *user_data);

otc_bool get_video_capturer_capture_settings(const otc_video_capturer *capturer,
                                                    void *user_data,
                                                    struct otc_video_capturer_settings *settings);
enum class DIRECTION {
    LEFT,
    RIGHT
};

cv::Rect get_eye_pos(DIRECTION eye);

void wait_till_stopped();

void wait_till_moving();

#endif //ODOCS_GANTRY_VIDEO_PROCESSING_H
