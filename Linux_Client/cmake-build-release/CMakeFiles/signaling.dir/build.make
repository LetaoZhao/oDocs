# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/246/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/246/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/signaling.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/signaling.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/signaling.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/signaling.dir/flags.make

CMakeFiles/signaling.dir/src/main.cpp.o: CMakeFiles/signaling.dir/flags.make
CMakeFiles/signaling.dir/src/main.cpp.o: /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/main.cpp
CMakeFiles/signaling.dir/src/main.cpp.o: CMakeFiles/signaling.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/signaling.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/signaling.dir/src/main.cpp.o -MF CMakeFiles/signaling.dir/src/main.cpp.o.d -o CMakeFiles/signaling.dir/src/main.cpp.o -c /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/main.cpp

CMakeFiles/signaling.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/signaling.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/main.cpp > CMakeFiles/signaling.dir/src/main.cpp.i

CMakeFiles/signaling.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/signaling.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/main.cpp -o CMakeFiles/signaling.dir/src/main.cpp.s

CMakeFiles/signaling.dir/src/gantry_interface.cpp.o: CMakeFiles/signaling.dir/flags.make
CMakeFiles/signaling.dir/src/gantry_interface.cpp.o: /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/gantry_interface.cpp
CMakeFiles/signaling.dir/src/gantry_interface.cpp.o: CMakeFiles/signaling.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/signaling.dir/src/gantry_interface.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/signaling.dir/src/gantry_interface.cpp.o -MF CMakeFiles/signaling.dir/src/gantry_interface.cpp.o.d -o CMakeFiles/signaling.dir/src/gantry_interface.cpp.o -c /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/gantry_interface.cpp

CMakeFiles/signaling.dir/src/gantry_interface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/signaling.dir/src/gantry_interface.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/gantry_interface.cpp > CMakeFiles/signaling.dir/src/gantry_interface.cpp.i

CMakeFiles/signaling.dir/src/gantry_interface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/signaling.dir/src/gantry_interface.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/gantry_interface.cpp -o CMakeFiles/signaling.dir/src/gantry_interface.cpp.s

CMakeFiles/signaling.dir/src/otk_thread.c.o: CMakeFiles/signaling.dir/flags.make
CMakeFiles/signaling.dir/src/otk_thread.c.o: /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/otk_thread.c
CMakeFiles/signaling.dir/src/otk_thread.c.o: CMakeFiles/signaling.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/signaling.dir/src/otk_thread.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/signaling.dir/src/otk_thread.c.o -MF CMakeFiles/signaling.dir/src/otk_thread.c.o.d -o CMakeFiles/signaling.dir/src/otk_thread.c.o -c /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/otk_thread.c

CMakeFiles/signaling.dir/src/otk_thread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/signaling.dir/src/otk_thread.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/otk_thread.c > CMakeFiles/signaling.dir/src/otk_thread.c.i

CMakeFiles/signaling.dir/src/otk_thread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/signaling.dir/src/otk_thread.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/otk_thread.c -o CMakeFiles/signaling.dir/src/otk_thread.c.s

CMakeFiles/signaling.dir/src/renderer.cpp.o: CMakeFiles/signaling.dir/flags.make
CMakeFiles/signaling.dir/src/renderer.cpp.o: /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/renderer.cpp
CMakeFiles/signaling.dir/src/renderer.cpp.o: CMakeFiles/signaling.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/signaling.dir/src/renderer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/signaling.dir/src/renderer.cpp.o -MF CMakeFiles/signaling.dir/src/renderer.cpp.o.d -o CMakeFiles/signaling.dir/src/renderer.cpp.o -c /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/renderer.cpp

CMakeFiles/signaling.dir/src/renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/signaling.dir/src/renderer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/renderer.cpp > CMakeFiles/signaling.dir/src/renderer.cpp.i

CMakeFiles/signaling.dir/src/renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/signaling.dir/src/renderer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/src/renderer.cpp -o CMakeFiles/signaling.dir/src/renderer.cpp.s

# Object files for target signaling
signaling_OBJECTS = \
"CMakeFiles/signaling.dir/src/main.cpp.o" \
"CMakeFiles/signaling.dir/src/gantry_interface.cpp.o" \
"CMakeFiles/signaling.dir/src/otk_thread.c.o" \
"CMakeFiles/signaling.dir/src/renderer.cpp.o"

# External object files for target signaling
signaling_EXTERNAL_OBJECTS =

signaling: CMakeFiles/signaling.dir/src/main.cpp.o
signaling: CMakeFiles/signaling.dir/src/gantry_interface.cpp.o
signaling: CMakeFiles/signaling.dir/src/otk_thread.c.o
signaling: CMakeFiles/signaling.dir/src/renderer.cpp.o
signaling: CMakeFiles/signaling.dir/build.make
signaling: /home/tazukiswift/Work/Prog/toolchains/libopentok_linux_llvm_x86_64/lib/libopentok.so
signaling: CMakeFiles/signaling.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable signaling"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/signaling.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/signaling.dir/build: signaling
.PHONY : CMakeFiles/signaling.dir/build

CMakeFiles/signaling.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/signaling.dir/cmake_clean.cmake
.PHONY : CMakeFiles/signaling.dir/clean

CMakeFiles/signaling.dir/depend:
	cd /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release /home/tazukiswift/Work/Prog/p4p27_python_client/Linux_Client/cmake-build-release/CMakeFiles/signaling.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/signaling.dir/depend

