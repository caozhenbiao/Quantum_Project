# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0"

# Include any dependencies generated for this target.
include tests/libtest/CMakeFiles/libauthretry.dir/depend.make

# Include the progress variables for this target.
include tests/libtest/CMakeFiles/libauthretry.dir/progress.make

# Include the compile flags for this target's objects.
include tests/libtest/CMakeFiles/libauthretry.dir/flags.make

tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o: tests/libtest/CMakeFiles/libauthretry.dir/flags.make
tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o: tests/libtest/libauthretry.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/libauthretry.dir/libauthretry.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/libauthretry.c"

tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libauthretry.dir/libauthretry.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/libauthretry.c" > CMakeFiles/libauthretry.dir/libauthretry.c.i

tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libauthretry.dir/libauthretry.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/libauthretry.c" -o CMakeFiles/libauthretry.dir/libauthretry.c.s

tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.requires

tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.provides: tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/libauthretry.dir/build.make tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.provides

tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.provides.build: tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o


tests/libtest/CMakeFiles/libauthretry.dir/first.c.o: tests/libtest/CMakeFiles/libauthretry.dir/flags.make
tests/libtest/CMakeFiles/libauthretry.dir/first.c.o: tests/libtest/first.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/libtest/CMakeFiles/libauthretry.dir/first.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/libauthretry.dir/first.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/first.c"

tests/libtest/CMakeFiles/libauthretry.dir/first.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libauthretry.dir/first.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/first.c" > CMakeFiles/libauthretry.dir/first.c.i

tests/libtest/CMakeFiles/libauthretry.dir/first.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libauthretry.dir/first.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/first.c" -o CMakeFiles/libauthretry.dir/first.c.s

tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.requires

tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.provides: tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/libauthretry.dir/build.make tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.provides

tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.provides.build: tests/libtest/CMakeFiles/libauthretry.dir/first.c.o


# Object files for target libauthretry
libauthretry_OBJECTS = \
"CMakeFiles/libauthretry.dir/libauthretry.c.o" \
"CMakeFiles/libauthretry.dir/first.c.o"

# External object files for target libauthretry
libauthretry_EXTERNAL_OBJECTS =

tests/libtest/libauthretry: tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o
tests/libtest/libauthretry: tests/libtest/CMakeFiles/libauthretry.dir/first.c.o
tests/libtest/libauthretry: tests/libtest/CMakeFiles/libauthretry.dir/build.make
tests/libtest/libauthretry: lib/libcurl.so
tests/libtest/libauthretry: /usr/lib/x86_64-linux-gnu/libssl.so
tests/libtest/libauthretry: /usr/lib/x86_64-linux-gnu/libcrypto.so
tests/libtest/libauthretry: /usr/lib/x86_64-linux-gnu/libz.so
tests/libtest/libauthretry: tests/libtest/CMakeFiles/libauthretry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable libauthretry"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libauthretry.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/libtest/CMakeFiles/libauthretry.dir/build: tests/libtest/libauthretry

.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/build

tests/libtest/CMakeFiles/libauthretry.dir/requires: tests/libtest/CMakeFiles/libauthretry.dir/libauthretry.c.o.requires
tests/libtest/CMakeFiles/libauthretry.dir/requires: tests/libtest/CMakeFiles/libauthretry.dir/first.c.o.requires

.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/requires

tests/libtest/CMakeFiles/libauthretry.dir/clean:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && $(CMAKE_COMMAND) -P CMakeFiles/libauthretry.dir/cmake_clean.cmake
.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/clean

tests/libtest/CMakeFiles/libauthretry.dir/depend:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/CMakeFiles/libauthretry.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : tests/libtest/CMakeFiles/libauthretry.dir/depend

