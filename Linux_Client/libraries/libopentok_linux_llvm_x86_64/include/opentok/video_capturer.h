/** @file video_capturer.h
    @brief Video capturer.

    This file includes the type definition for a video capturer along with
    several function declarations useful when working with it.
*/
#ifndef VIDEO_CAPTURER_H
#define VIDEO_CAPTURER_H

#include "config.h"
#include "video_frame.h"

OTC_BEGIN_DECL

/** Video capturer type definition.
 */
typedef struct otc_video_capturer otc_video_capturer;

/** Video capturer settings.

    This struct represents the settings associated with a video capturer.
 */
struct otc_video_capturer_settings {
  int format; /**< The pixel format. */
  int width;  /**< The width of the video, in pixels. */
  int height; /**< The height of the video, in pixels. */
  int fps;    /**< The estimated number of frames per second in the video. */
  int expected_delay; /**< The estimated capture delay, in milliseconds, of the
                         video. */
  otc_bool mirror_on_local_render; /**< Whether the frame should appear mirrored
                                      on the x-axis in the local renderer. */
};

/** Audio device callback functions.

    This struct is a set of function pointers to callback functions for events
    related to a video capturer.

    All callbacks will not be made on the application or main thread but on an
    internal thread. The application should return the callback as quickly as
    possible to avoid blocking the internal thread.
 */
struct otc_video_capturer_callbacks {
  /**
      Requests that a video capturer initialize itself.

      @param capturer A pointer to a video capturer instance.
      @param user_data Pointer to user custom data bound to this struct.
      @return Boolean value indicating either error or success.
      @see otc_bool
   */
  otc_bool (*init)(const otc_video_capturer* capturer, void* user_data);

  /**
      Requests that a video capturer destroy itself.

      @param capturer A pointer to a video capturer instance.
      @param user_data Pointer to user custom data bound to this struct.
      @return Boolean value indicating either error or success.
      @see otc_bool
   */
  otc_bool (*destroy)(const otc_video_capturer* capturer, void* user_data);

  /**
      Requests that the video capturer starts capturing video.

      @param capturer A pointer to a video capturer instance.
      @param user_data Pointer to user custom data bound to this struct.
      @return Boolean value indicating either error or success.
      @see otc_bool
   */
  otc_bool (*start)(const otc_video_capturer* capturer, void* user_data);

  /**
      Requests that the video capturer stops capturing video.

      @param capturer A pointer to a video capturer instance.
      @param user_data Pointer to user custom data bound to this struct.
      @return Boolean value indicating either error or success.
      @see otc_bool
   */
  otc_bool (*stop)(const otc_video_capturer* capturer, void* user_data);

  /**
      Request that the video capturer returns the settings being used.

      @param capturer A pointer to a video capturer instance.
      @param user_data Pointer to user custom data bound to this struct.
      @param settings A pointer to a struct holding the settings.
      @return Boolean value indicating either error or success.
      @see otc_bool
   */
  otc_bool (*get_capture_settings)(
      const otc_video_capturer* capturer,
      void* user_data,
      struct otc_video_capturer_settings* settings);

  /**
    This struct member can point to data related to the video capturer
    which the developer might need.
  */
  void* user_data;

  /**
      A void pointer to a memory area holding reserved resources used for the
      internal implementation.
   */
  void* reserved;
};

/**
    Provides a video frame to the video capturer.

    @param capturer A pointer to the video capturer instance.
    @param rotation The rotation of the video frame.
    @param frame A pointer to the video frame instance to be provided.
    @return Return value indicating either error or success.
    @see otc_video_frame
 */
OTC_DECL(otc_status)
otc_video_capturer_provide_frame(const otc_video_capturer* capturer,
                                 int rotation,
                                 const otc_video_frame* frame);

/**
   Defines values for the <code>content_hint</code> parameter of
   the {@link otc_video_capturer_set_content_hint} function.
 */
enum otc_video_capturer_content_hint_type {
  OTC_VIDEO_CAPTURER_CONTENT_HINT_TYPE_NONE =
      0, /**< No hint provided. Default value "none" will be used. */
  OTC_VIDEO_CAPTURER_CONTENT_HINT_TYPE_MOTION = 1, /**< value = "motion" */
  OTC_VIDEO_CAPTURER_CONTENT_HINT_TYPE_DETAIL = 2, /**< value = "detail". */
  OTC_VIDEO_CAPTURER_CONTENT_HINT_TYPE_TEXT = 3,   /**< value = "text". */
};

/**
 * Sets the video content hint to one of the enum values in
 * otc_video_capturer_content_hint_type. If the API receives any value other
 * than the enum values specified above, then we log a warning about the same.
 * @param capturer The pointer to the capturer
 * @param content_hint Enum value of type otc_video_capturer_content_hint_type.
 * @return Return value indicating either failure or success.
 * */
OTC_DECL(otc_status)
const otc_video_capturer_set_content_hint(
    const otc_video_capturer* capturer,
    enum otc_video_capturer_content_hint_type content_hint);

/** Returns the content_hint value for the capturer video source.
 *  @param capturer The pointer to the capturer.
 *  @return Enum value of type otc_video_capturer_content_hint_type
 */
OTC_DECL(enum otc_video_capturer_content_hint_type)
const otc_video_capturer_get_content_hint(const otc_video_capturer* capturer);

OTC_END_DECL

#endif  // VIDEO_CAPTURER_H
