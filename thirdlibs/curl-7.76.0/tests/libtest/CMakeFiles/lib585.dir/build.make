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
include tests/libtest/CMakeFiles/lib585.dir/depend.make

# Include the progress variables for this target.
include tests/libtest/CMakeFiles/lib585.dir/progress.make

# Include the compile flags for this target's objects.
include tests/libtest/CMakeFiles/lib585.dir/flags.make

tests/libtest/CMakeFiles/lib585.dir/lib500.c.o: tests/libtest/CMakeFiles/lib585.dir/flags.make
tests/libtest/CMakeFiles/lib585.dir/lib500.c.o: tests/libtest/lib500.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/libtest/CMakeFiles/lib585.dir/lib500.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib585.dir/lib500.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/lib500.c"

tests/libtest/CMakeFiles/lib585.dir/lib500.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib585.dir/lib500.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/lib500.c" > CMakeFiles/lib585.dir/lib500.c.i

tests/libtest/CMakeFiles/lib585.dir/lib500.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib585.dir/lib500.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/lib500.c" -o CMakeFiles/lib585.dir/lib500.c.s

tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.requires

tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.provides: tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/lib585.dir/build.make tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.provides

tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.provides.build: tests/libtest/CMakeFiles/lib585.dir/lib500.c.o


tests/libtest/CMakeFiles/lib585.dir/first.c.o: tests/libtest/CMakeFiles/lib585.dir/flags.make
tests/libtest/CMakeFiles/lib585.dir/first.c.o: tests/libtest/first.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/libtest/CMakeFiles/lib585.dir/first.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib585.dir/first.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/first.c"

tests/libtest/CMakeFiles/lib585.dir/first.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib585.dir/first.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/first.c" > CMakeFiles/lib585.dir/first.c.i

tests/libtest/CMakeFiles/lib585.dir/first.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib585.dir/first.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/first.c" -o CMakeFiles/lib585.dir/first.c.s

tests/libtest/CMakeFiles/lib585.dir/first.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/lib585.dir/first.c.o.requires

tests/libtest/CMakeFiles/lib585.dir/first.c.o.provides: tests/libtest/CMakeFiles/lib585.dir/first.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/lib585.dir/build.make tests/libtest/CMakeFiles/lib585.dir/first.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/lib585.dir/first.c.o.provides

tests/libtest/CMakeFiles/lib585.dir/first.c.o.provides.build: tests/libtest/CMakeFiles/lib585.dir/first.c.o


tests/libtest/CMakeFiles/lib585.dir/testutil.c.o: tests/libtest/CMakeFiles/lib585.dir/flags.make
tests/libtest/CMakeFiles/lib585.dir/testutil.c.o: tests/libtest/testutil.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object tests/libtest/CMakeFiles/lib585.dir/testutil.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib585.dir/testutil.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/testutil.c"

tests/libtest/CMakeFiles/lib585.dir/testutil.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib585.dir/testutil.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/testutil.c" > CMakeFiles/lib585.dir/testutil.c.i

tests/libtest/CMakeFiles/lib585.dir/testutil.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib585.dir/testutil.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/testutil.c" -o CMakeFiles/lib585.dir/testutil.c.s

tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.requires

tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.provides: tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/lib585.dir/build.make tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.provides

tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.provides.build: tests/libtest/CMakeFiles/lib585.dir/testutil.c.o


tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o: tests/libtest/CMakeFiles/lib585.dir/flags.make
tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o: tests/libtest/testtrace.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib585.dir/testtrace.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/testtrace.c"

tests/libtest/CMakeFiles/lib585.dir/testtrace.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib585.dir/testtrace.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/testtrace.c" > CMakeFiles/lib585.dir/testtrace.c.i

tests/libtest/CMakeFiles/lib585.dir/testtrace.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib585.dir/testtrace.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/testtrace.c" -o CMakeFiles/lib585.dir/testtrace.c.s

tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.requires

tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.provides: tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/lib585.dir/build.make tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.provides

tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.provides.build: tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o


tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o: tests/libtest/CMakeFiles/lib585.dir/flags.make
tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o: lib/curl_multibyte.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building C object tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o   -c "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/lib/curl_multibyte.c"

tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.i"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/lib/curl_multibyte.c" > CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.i

tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.s"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/lib/curl_multibyte.c" -o CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.s

tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.requires:

.PHONY : tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.requires

tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.provides: tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.requires
	$(MAKE) -f tests/libtest/CMakeFiles/lib585.dir/build.make tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.provides.build
.PHONY : tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.provides

tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.provides.build: tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o


# Object files for target lib585
lib585_OBJECTS = \
"CMakeFiles/lib585.dir/lib500.c.o" \
"CMakeFiles/lib585.dir/first.c.o" \
"CMakeFiles/lib585.dir/testutil.c.o" \
"CMakeFiles/lib585.dir/testtrace.c.o" \
"CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o"

# External object files for target lib585
lib585_EXTERNAL_OBJECTS =

tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/lib500.c.o
tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/first.c.o
tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/testutil.c.o
tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o
tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o
tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/build.make
tests/libtest/lib585: lib/libcurl.so
tests/libtest/lib585: /usr/lib/x86_64-linux-gnu/libssl.so
tests/libtest/lib585: /usr/lib/x86_64-linux-gnu/libcrypto.so
tests/libtest/lib585: /usr/lib/x86_64-linux-gnu/libz.so
tests/libtest/lib585: tests/libtest/CMakeFiles/lib585.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable lib585"
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lib585.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/libtest/CMakeFiles/lib585.dir/build: tests/libtest/lib585

.PHONY : tests/libtest/CMakeFiles/lib585.dir/build

tests/libtest/CMakeFiles/lib585.dir/requires: tests/libtest/CMakeFiles/lib585.dir/lib500.c.o.requires
tests/libtest/CMakeFiles/lib585.dir/requires: tests/libtest/CMakeFiles/lib585.dir/first.c.o.requires
tests/libtest/CMakeFiles/lib585.dir/requires: tests/libtest/CMakeFiles/lib585.dir/testutil.c.o.requires
tests/libtest/CMakeFiles/lib585.dir/requires: tests/libtest/CMakeFiles/lib585.dir/testtrace.c.o.requires
tests/libtest/CMakeFiles/lib585.dir/requires: tests/libtest/CMakeFiles/lib585.dir/__/__/lib/curl_multibyte.c.o.requires

.PHONY : tests/libtest/CMakeFiles/lib585.dir/requires

tests/libtest/CMakeFiles/lib585.dir/clean:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" && $(CMAKE_COMMAND) -P CMakeFiles/lib585.dir/cmake_clean.cmake
.PHONY : tests/libtest/CMakeFiles/lib585.dir/clean

tests/libtest/CMakeFiles/lib585.dir/depend:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/libtest/CMakeFiles/lib585.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : tests/libtest/CMakeFiles/lib585.dir/depend

