CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(odocs_gantry)

SET(CMAKE_VERBOSE_MAKEFILE ON)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")

# Remove the commented out line below to enable OpenTok debug traces.
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCONSOLE_LOGGING")

SET(CMAKE_BUILD_TYPE Debug)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(LIBSDL2 REQUIRED sdl2)

find_package( OpenCV REQUIRED )

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/libraries/opencv_build)

set(ENV{LIBOPENTOK_PATH} ${CMAKE_SOURCE_DIR}/libraries/libopentok_linux_llvm_x86_64/)

if (DEFINED ENV{LIBOPENTOK_PATH})
    find_path(LIBOPENTOK_HEADER opentok.h PATHS $ENV{LIBOPENTOK_PATH}/include NO_DEFAULT_PATH)
    find_library(LIBOPENTOK_LIBRARIES libopentok NAMES libopentok.so PATHS $ENV{LIBOPENTOK_PATH}/lib NO_DEFAULT_PATH)
endif ()

if (NOT LIBOPENTOK_LIBRARIES AND NOT LIBOPENTOK_HEADER)
    PKG_SEARCH_MODULE(LIBOPENTOK REQUIRED libopentok)
else ()
    set(LIBOPENTOK_LIBRARY_DIRS $ENV{LIBOPENTOK_PATH}/lib)
    set(LIBOPENTOK_INCLUDE_DIRS $ENV{LIBOPENTOK_PATH}/include)
endif ()

FIND_PACKAGE(Threads)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/./libs/common/src ${LIBSDL2_INCLUDE_DIRS} ${LIBOPENTOK_INCLUDE_DIRS} ${serial_INCLUDE_DIRS}/serial/ ${OpenCV_INCLUDE_DIRS} include)

LINK_DIRECTORIES(${LIBSDL2_LIBRARY_DIRS} ${LIBOPENTOK_LIBRARY_DIRS} ${serial_LIBRARY_DIRS})

ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp src/gantry_interface.cpp src/otk_thread.cpp src/renderer.cpp src/renderer.cpp src/renderer.cpp src/error_handler.cpp
        src/video_processing.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIBSDL2_LIBRARIES} ${LIBOPENTOK_LIBRARIES} ${serial_LIBRARIES} ${OpenCV_LIBS})

get_cmake_property(_variableNames VARIABLES)
list(SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach ()

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()