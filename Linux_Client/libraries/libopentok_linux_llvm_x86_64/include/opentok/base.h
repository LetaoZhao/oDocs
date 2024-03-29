/** @file base.h
    @brief Base functions.

    This file includes functions uses to initialize and destroy the library
    engine, as well as other functions to enable console logging.
*/
#ifndef BASE_H
#define BASE_H

#include "config.h"

OTC_BEGIN_DECL

/** Enum for reasons for a video to be started, stopped, resumed, etc.
 */
enum otc_video_reason {
  OTC_VIDEO_REASON_PUBLISH_VIDEO =
      1, /**< The client publishing the stream stopped streaming video. */
  OTC_VIDEO_REASON_SUBSCRIBE_TO_VIDEO =
      2, /**< The local client stopped subscribing to video. */
  OTC_VIDEO_REASON_QUALITY = 3, /**< The video event was caused by a change to
the video stream quality. */
  OTC_VIDEO_REASON_CODEC_NOT_SUPPORTED = 4 /**< The video event was caused when
    video in the subscriber stream was disabled because the stream uses a video
    codec that is not supported on the device. */
};

/**
    This enumeration includes commonly used constants.
 */
enum otc_constant {
  OTC_SUCCESS = 0,      /**< Constant that represents success. */
  OTC_FALSE = 0,        /**< Constant that represents false. */
  OTC_TRUE = !OTC_FALSE /**< Constant that represents true. */
};

/**
    This enumeration includes error codes.
 */
enum otc_error_code {
  /** Invalid funtion argument error code.

      An argment used in a function call is not valid.
  */
  OTC_INVALID_PARAM = 1,
  /** Fatal error code.

      Generic error.
  */
  OTC_FATAL = 2,
  /** Invalid video format

      Raised when the capturer is configured with an invalid video format
  */
  OTC_INVALID_VIDEO_FORMAT = 3,
  /** Connection dropped error code.

     The connection to the OpenTok messaging server was dropped. Check the
     network connection.
   */
  OTC_CONNECTION_DROPPED = 1022,
  /** Force mute error code.

      Unable to successfully  call {@link otc_session_force_mute_all} or
      {@link otc_session_force_mute_stream}, perhaps because of missing
      permissions.
   */
  OTC_UNABLE_TO_FORCE_MUTE = 1540,
  /** Connection time out error code.

      Time out while performing a connect action.
   */
  OTC_CONNECTION_TIMED_OUT = 1542,
  /** Unknown publisher instance error code.

      An unknown Publisher instance was used as a function argument.
   */
  OTC_UNKNOWN_PUBLISHER_INSTANCE = 2003,
  /** Unknown subscriber instance error code.

      An unknown subscriber instance was used as a function argument.
   */
  OTC_UNKNOWN_SUBSCRIBER_INSTANCE = 2004,
  /** Video capture error code.

      There was an error with the video capturer.
   */
  OTC_VIDEO_CAPTURE_FAILED = 3000,
  /** Acquiring video camera error code.

      There was an error while acquiring video from the camera.
   */
  OTC_CAMERA_FAILED = 3010,
  /** Video rendering error code.

      There was an error while rendering video.
   */
  OTC_VIDEO_RENDER_FAILED = 4000,
  /** Media engine access error code.

      There was an error when trying to get the list of supported video codecs.
   */
  OT_UNABLE_TO_ACCESS_MEDIA_ENGINE = 5000,
};

/** Log level enumeration.

    This enumeration represents the different log levels supported.
 */
enum otc_log_level {
  OTC_LOG_LEVEL_DISABLED = 0, /**< No messages enum value. */
  OTC_LOG_LEVEL_ERROR = 1, /**< Error level messages. */
  OTC_LOG_LEVEL_WARN = 2,   /**< Warning level messages. */
  OTC_LOG_LEVEL_INFO = 3,   /**< Info level messages. */
  OTC_LOG_LEVEL_DEBUG = 4, /**< Debug level messages. */

  OTC_LOG_LEVEL_FATAL = OTC_LOG_LEVEL_ERROR,    /**< DEPRECATED. */
  OTC_LOG_LEVEL_MSG = OTC_LOG_LEVEL_DEBUG,      /**< DEPRECATED. */
  OTC_LOG_LEVEL_TRACE = OTC_LOG_LEVEL_DEBUG,    /**< DEPRECATED. */
  OTC_LOG_LEVEL_ALL = OTC_LOG_LEVEL_DEBUG       /**< DEPRECATED. */
};

/** Type definition for Boolean values.

    Useful type for variables to represent Boolean values (true and false).
 */
typedef int otc_bool;

/**
  Type definition additional details to be provided in the
  {@link otc_session_callbacks.on_mute_forced} function.
 */
typedef struct otc_on_mute_forced_info {
  /**
  Set to <code>OTC_TRUE</code> when a moderator has muted audio in the session; Set to
  <code>OTC_FALSE</code> when a moderator disables the mute audio state in the session.
  */
  otc_bool active;
} otc_on_mute_forced_info;

/**
    Type definition for statuses.

    This is the most common type for return value for functions in this
    library. This type includes values from the error code enumerations.

    @see otc_constant
    @see otc_error_code
    @see otc_session_error_code
    @see otc_publisher_error_code
    @see otc_subscriber_error_code
 */
typedef int otc_status;

/**
    Type definition for a function pointer to the function used as the
    custom logging callback to get messages logged by the SDK.
    @param message A pointer to the message being logged.
    @see otc_log_set_logger_callback
 */
typedef void (*otc_logger_callback)(const char* message);

/**
    Initializes the library. You must call this function before
    the execution of any other code using the library.
    @param reserved A void pointer to a memory area holding reserved resources
    used for the internal implementation. This must be set to <code>NULL</code>.
    @return Return value indicating either error or success.
 */
OTC_DECL(otc_status) otc_init(void* reserved);

/**
    Destroys the library engine. You should call this function when you are done
    executing code that uses the library.
    @return Return value indicating either error or success.
 */
OTC_DECL(otc_status) otc_destroy();

/**
    Sets the function pointer to a custom logging callback function to get
   messages logged by the SDK. If you do not set a logging callback function,
   the SDK will log messages to the console.
    @param callback A pointer to the callback function.
    @see otc_log_enable
 */
OTC_DECL(void) otc_log_set_logger_callback(otc_logger_callback callback);

/**
    Enables logging of SDK messages for a given log level.
    @param log_level The desired log level.
    @see otc_log_set_logger_callback
    @see otc_get_log_level
 */
OTC_DECL(void) otc_log_enable(enum otc_log_level log_level);

/**

    Gets the current console logging level.
    @return The log level set.
 */
OTC_DECL(enum otc_log_level) otc_get_log_level();

OTC_END_DECL

#endif  // BASE_H
