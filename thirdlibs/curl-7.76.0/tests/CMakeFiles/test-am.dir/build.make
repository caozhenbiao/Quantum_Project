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

# Utility rule file for test-am.

# Include the progress variables for this target.
include tests/CMakeFiles/test-am.dir/progress.make

tests/CMakeFiles/test-am:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests" && /usr/bin/perl "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/runtests.pl" -a -am "\$$TFLAGS"

test-am: tests/CMakeFiles/test-am
test-am: tests/CMakeFiles/test-am.dir/build.make

.PHONY : test-am

# Rule to build all files generated by this target.
tests/CMakeFiles/test-am.dir/build: test-am

.PHONY : tests/CMakeFiles/test-am.dir/build

tests/CMakeFiles/test-am.dir/clean:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests" && $(CMAKE_COMMAND) -P CMakeFiles/test-am.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test-am.dir/clean

tests/CMakeFiles/test-am.dir/depend:
	cd "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests" "/home/caozhenbiao/Quantum Project 1.0/thirdlibs/curl-7.76.0/tests/CMakeFiles/test-am.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : tests/CMakeFiles/test-am.dir/depend
