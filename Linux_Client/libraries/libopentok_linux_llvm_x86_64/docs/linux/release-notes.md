# OpenTok Linux SDK release notes

## New features and changes

### Version 2.25.1 -- May 2023

* This version fixes some memory leaks.

* This version fixes an issue where occasionally the app would crash if it was publishing for a very 
  long time.  

### Version 2.25.0 -- April 2023

* This version removes support for Debian 10.

* This version of the SDK uses an updated version of WebRTC 99, which includes improvements in performance and stability.

* This version fixes an issue where 1080p was only supported if it was the first camera found.

### Version 2.24.3 -- February 2023

* This version adds support for network proxies. This is a beta feature.

* This version fixes an issue where starting a session with muted video would disable scalable video. 

* This version fixes an issue where the camera LED was not turned off when the video was muted. 

* This version fixes an issue where audio would remain muted for users under certain circumstances.

### Version 2.24.2 -- December 2022

* This version fixes an issue where, in some cases, using a proxy server would cause the application to crash. 

### Version 2.24.1 -- November 2022

* The version fixes some dependency issues in 2.24.0. 

### Version 2.24.0 -- November 2022

* This version of the SDK uses WebRTC 99, which includes improvements in performance and stability.

* This version adds the ability to enable per-subscriber audio levels. See the docs for the 
  `otc_subscriber_set_audio_volume()` and `otc_subscriber_get_audio_volume()` functions.

* This version fixes an issue where bitrates were previously unconstrained in relayed sessions. 

* This version fixes an issue in which applications could not connect to a session when IP Proxy
  was enabled.

* This version fixes an issue when AES-256 was enabled and applications could not connect to
  clients using on Safari 15.3 and below.

### Version 2.23.0 -- June 2022

* This version adds bandwidth consumption improvements in relayed sessions.

* This version adds support for Debian 11, and removes support for Debian 9.

* Adding support for Full High Definition (1920x1080-pixel) resolution. You can specify  
  `OTC_CAMERA_CAPTURE_RESOLUTION_1080P` (from the `otc_camera_capture_resolution` enum) as the
  `otc_camera_capture_resolution` parameter you pass into the `otc_publisher_settings_set_default_camera_capture_resolution()`
  function. 

* This version adds the ability to enable per-subscriber audio levels. See the docs for the 
  `otc_subscriber_set_audio_volume()` and `otc_subscriber_get_audio_volume()` functions.  This was previously a beta feature.

* This version implements [scalable video](https://tokbox.com/developer/guides/scalable-video/) support for screen sharing. 
  See the `otc_publisher_settings_set_scalable_screenshare()` function. This was previously a beta feature.

* This version fixes an issue where publishers of 1080p or 720p streams to
  [routed sessions](https://tokbox.com/developer/guides/create-session/#media-mode)
  published two [scalable video](https://tokbox.com/developer/guides/scalable-video/)
  layers instead of three.

* This version fixes an issue where the default video renderer fails when the requested resolution is not available. 

### Version 2.22.3 -- May 2022

* This version removes some spurious logging. 

### Version 2.22.2 -- April 2022

* This version adds the ability to enable per-subscriber audio levels. See the docs for
  the `otc_subscriber_set_audio_volume()` and `otc_subscriber_get_audio_volume()` functions.  This is a beta feature.

* This version implements [scalable video](https://tokbox.com/developer/guides/scalable-video/) support for 
  screen sharing. See the `otc_publisher_settings_set_scalable_screenshare()` function. This is a beta feature.

### Version 2.22.1 -- March 2022

* This version removes support for DTLS 1.0.

### Version 2.22.0 -- March 2022

* This version adds the ability to enable [Opus DTX](https://datatracker.ietf.org/doc/html/rfc7587#section-3.1.3).
  Enabling Opus DTX can reduce bandwidth usage in streams that have long periods of silence.
  This was previously a beta feature. See the docs for the `otc_publisher_settings_set_opus_dtx()`
  function.

* This version adds the ability to turn off audio processing. See the docs for the
  `otc_publisher_settings_set_auto_gain_control()`, `otc_publisher_settings_set_noise_suppression()`,
  and `otc_publisher_settings_set_echo_cancellation()` functions.

* This version adds the ability to get subscriber audio data. See the docs for
  the `on_audio_data` callback function of the `otc_subscriber_callbacks` struct.

* This version removes support for cipher TLS_RSA_WITH_3DES_EDE_CBC_SHA.

### Version 2.21.2 -- November 2021

* This version fixes an issue in relayed sessions where the audio bitrate set with
  the `otc_publisher_set_max_audio_bitrate()` function was not being respected.

### Version 2.21.1 -- October 2021

* This version adds the ability to enable [Opus DTX](https://datatracker.ietf.org/doc/html/rfc7587#section-3.1.3).
  Enabling Opus DTX can reduce bandwidth usage in streams that have long periods of silence.
  This is a <i>beta</i> feature. See the docs for the `otc_publisher_settings_set_opus_dtx()`
  function.

### Version 2.21.0 -- October 2021

* This version adds the ability to dynamically change how published video streams are optimized.
  This can improve video performance for some screen-sharing streams and in other conditions.
  See the docs for the `otc_video_capturer_get_content_hint()` and `otc_video_capturer_set_content_hint()`
  functions.

* This version adds the ability for moderators to mute participants. This was previously a
  _beta_ feature. See the docs for the `otc_session_force_mute_all()`, `otc_session_force_mute_stream()`,
  and `otc_session_disable_force_mute()` functions. Note that the `otc_session_force_mute_all()` and
  `otc_session_force_mute_stream()` functions no longer include an `options` parameter, like they did
  in the beta version.

* This version fixes an issue where an app may crash when gathering audio and video statistics for a subscriber.

### Version 2.20.0 -- July 2021

* This version adds the ability for moderators to mute participants. This is a *beta*
  feature. See the docs for the `otc_session_force_mute_all()` and `otc_session_force_mute_stream()` methods.

* This version fixes an issue where the `otc_session_unpublish()` method may fail if called immediately after publishing.

### Version 2.19.1 -- April 2021

* This version fixes an issue where a potential memory leak may occur when publishing stops.

### Version 2.19.0 -- February 2021

* This version of the SDK uses WebRTC 84, which includes improvements in performance and stability.

* This version adds support for RTC audio and video statistics for publishers and subscribers.
  See the docs for the  `otc_publisher_get_rtc_stats_report()` and `otc_subscriber_get_rtc_stats_report()`
  functions.

* This version fixes an issue where the audio bitrate value set for publishers was handled incorrectly internally.

* The [automatic reconnection feature](https://tokbox.com/developer/guides/connect-session/linux/#automatic_reconnection)
  is supported from this version onward.

### Version 2.18.1 -- October 2020

This version fixes the following issues:

* The `on_error` callback function in the `otc_publisher_callbacks` struct
  was not being triggered in all scenarios where publishing failed.

* The SDK was not proxying all communications correctly when the IP Proxy feature was being used.

### Version 2.18.0 -- August 2020

This is the first public release of OpenTok Linux SDK.

## Known issues

The following are known issues:

* If a system proxy is configured, the SDK will not use it.

* The H.264 codec is not supported. See the [video codecs](https://tokbox.com/developer/guides/codecs/)
  developer guide.
