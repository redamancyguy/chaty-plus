# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = "/Users/sunwenli/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/sunwenli/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/server_plus.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/server_plus.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_plus.dir/flags.make

CMakeFiles/server_plus.dir/main.c.o: CMakeFiles/server_plus.dir/flags.make
CMakeFiles/server_plus.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server_plus.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server_plus.dir/main.c.o -c /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/main.c

CMakeFiles/server_plus.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server_plus.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/main.c > CMakeFiles/server_plus.dir/main.c.i

CMakeFiles/server_plus.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server_plus.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/main.c -o CMakeFiles/server_plus.dir/main.c.s

CMakeFiles/server_plus.dir/BufferQueue.c.o: CMakeFiles/server_plus.dir/flags.make
CMakeFiles/server_plus.dir/BufferQueue.c.o: ../BufferQueue.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/server_plus.dir/BufferQueue.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server_plus.dir/BufferQueue.c.o -c /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/BufferQueue.c

CMakeFiles/server_plus.dir/BufferQueue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server_plus.dir/BufferQueue.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/BufferQueue.c > CMakeFiles/server_plus.dir/BufferQueue.c.i

CMakeFiles/server_plus.dir/BufferQueue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server_plus.dir/BufferQueue.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/BufferQueue.c -o CMakeFiles/server_plus.dir/BufferQueue.c.s

CMakeFiles/server_plus.dir/Utils/Tree.c.o: CMakeFiles/server_plus.dir/flags.make
CMakeFiles/server_plus.dir/Utils/Tree.c.o: ../Utils/Tree.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/server_plus.dir/Utils/Tree.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server_plus.dir/Utils/Tree.c.o -c /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/Utils/Tree.c

CMakeFiles/server_plus.dir/Utils/Tree.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server_plus.dir/Utils/Tree.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/Utils/Tree.c > CMakeFiles/server_plus.dir/Utils/Tree.c.i

CMakeFiles/server_plus.dir/Utils/Tree.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server_plus.dir/Utils/Tree.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/Utils/Tree.c -o CMakeFiles/server_plus.dir/Utils/Tree.c.s

# Object files for target server_plus
server_plus_OBJECTS = \
"CMakeFiles/server_plus.dir/main.c.o" \
"CMakeFiles/server_plus.dir/BufferQueue.c.o" \
"CMakeFiles/server_plus.dir/Utils/Tree.c.o"

# External object files for target server_plus
server_plus_EXTERNAL_OBJECTS =

server_plus: CMakeFiles/server_plus.dir/main.c.o
server_plus: CMakeFiles/server_plus.dir/BufferQueue.c.o
server_plus: CMakeFiles/server_plus.dir/Utils/Tree.c.o
server_plus: CMakeFiles/server_plus.dir/build.make
server_plus: CMakeFiles/server_plus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable server_plus"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_plus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_plus.dir/build: server_plus
.PHONY : CMakeFiles/server_plus.dir/build

CMakeFiles/server_plus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_plus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_plus.dir/clean

CMakeFiles/server_plus.dir/depend:
	cd /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug /Users/sunwenli/Documents/GitHub/chaty-plus/server-plus/cmake-build-debug/CMakeFiles/server_plus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_plus.dir/depend

