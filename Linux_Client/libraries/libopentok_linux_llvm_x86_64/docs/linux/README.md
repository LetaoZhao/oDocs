# OpenTok Linux SDK

Apps written with the OpenTok Linux SDK 2.25.1 can interoperate with OpenTok apps
written with version 2.23+ of the OpenTok client SDKs:

* OpenTok.js
* OpenTok Android SDK
* OpenTok iOS SDK
* OpenTok macOS SDK
* OpenTok Windows SDK

The OpenTok Linux SDK supports applications written in C/C++.

## System requirements

The OpenTok Linux SDK works on desktops and laptops running Debian 11
(amd64). In addition to a Debian package, we provide tgz packages for development
on other platforms and architectures such as armv7 and arm64. However, Vonage
Support staff can only diagnose and troubleshoot issues that are reproducible on
desktops or laptops running Debian (amd64).

## Using the SDK

### Debian package

The OpenTok Linux SDK for x86_64 (adm64) architecture is available as a Debian
package. We support Debian 12 (bullseye). We maintain
our own Debian repository on packagecloud. Follow these steps
to install the packages from our repository.

* Add packagecloud repository:

  ```bash
  curl -s https://packagecloud.io/install/repositories/tokbox/debian/script.deb.sh | sudo bash
  ```

* Install the OpenTok Linux SDK packages.

  ```bash
  sudo apt-get install libopentok-dev
  ```

### `tgz` packages

The OpenTok Linux SDK for armv7 and arm64 architectures is available as a `tgz`
package via a direct download in the OpenTok developer center. There is also a
package for x86_64.

* For x86_64 architecture find the `tgz` package at https://tokbox.com/downloads/libopentok_linux_llvm_x86_64-2.25.1

* For armv7 architecture find the `tgz` package at https://tokbox.com/downloads/libopentok_linux_llvm_armv7-2.25.1

* For arm64 architecture find the `tgz` package at https://tokbox.com/downloads/libopentok_linux_llvm_arm64-2.25.1

You can download any of those packages and use them in your applications.

After extracting the packages the folder with the contents contains a handy
`CMakeLists.txt` file the developer can use if `CMake` is used in the
application. The developer would need to add it via the `ADD_SUBDIRECTORY()`
command, add `$<TARGET_PROPERTY:libopentok,INTERFACE_INCLUDE_DIRECTORIES>` to the
list of include folders with the `INCLUDE_DIRECTORIES()` command and finally add
`libopentok` to the list of target link libraries.

```
  ADD_SUBDIRECTORY(<absolute-path-to-package-folder> ${CMAKE_CURRENT_BINARY_DIR}/libopentok)

  # ...

  INCLUDE_DIRECTORIES(<other-include-directories> $<TARGET_PROPERTY:libopentok,INTERFACE_INCLUDE_DIRECTORIES>)

  # ...

  ADD_EXECUTABLE(${PROJECT_NAME} <sources>)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} <other-libraries> libopentok)
```

## Sample Applications

For sample code, go to the
[opentok-linux-sdk-samples repo](https://github.com/opentok/opentok-linux-sdk-samples)
at GitHub.

## Documentation

See the [OpenTok Linux SDK API reference](https://tokbox.com/developer/sdks/linux/reference/)
and the [OpenTok developer guides](https://tokbox.com/developer/guides/).

## More information

For a list of new features and known issues, see the [release notes](release-notes.md).
