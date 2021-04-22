# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10

# Include any dependencies generated for this target.
include modules/photo/CMakeFiles/opencv_photo.dir/depend.make

# Include the progress variables for this target.
include modules/photo/CMakeFiles/opencv_photo.dir/progress.make

# Include the compile flags for this target's objects.
include modules/photo/CMakeFiles/opencv_photo.dir/flags.make

modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o: modules/photo/CMakeFiles/opencv_photo.dir/flags.make
modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o: modules/photo/src/denoising.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/precomp.hpp" -Winvalid-pch  -o CMakeFiles/opencv_photo.dir/src/denoising.cpp.o -c /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/src/denoising.cpp

modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_photo.dir/src/denoising.cpp.i"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/precomp.hpp" -Winvalid-pch  -E /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/src/denoising.cpp > CMakeFiles/opencv_photo.dir/src/denoising.cpp.i

modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_photo.dir/src/denoising.cpp.s"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/precomp.hpp" -Winvalid-pch  -S /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/src/denoising.cpp -o CMakeFiles/opencv_photo.dir/src/denoising.cpp.s

modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.requires:
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.requires

modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.provides: modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.requires
	$(MAKE) -f modules/photo/CMakeFiles/opencv_photo.dir/build.make modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.provides.build
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.provides

modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.provides.build: modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.provides.build

modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o: modules/photo/CMakeFiles/opencv_photo.dir/flags.make
modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o: modules/photo/src/inpaint.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/precomp.hpp" -Winvalid-pch  -o CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o -c /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/src/inpaint.cpp

modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_photo.dir/src/inpaint.cpp.i"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/precomp.hpp" -Winvalid-pch  -E /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/src/inpaint.cpp > CMakeFiles/opencv_photo.dir/src/inpaint.cpp.i

modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_photo.dir/src/inpaint.cpp.s"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/precomp.hpp" -Winvalid-pch  -S /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/src/inpaint.cpp -o CMakeFiles/opencv_photo.dir/src/inpaint.cpp.s

modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.requires:
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.requires

modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.provides: modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.requires
	$(MAKE) -f modules/photo/CMakeFiles/opencv_photo.dir/build.make modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.provides.build
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.provides

modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.provides.build: modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.provides.build

# Object files for target opencv_photo
opencv_photo_OBJECTS = \
"CMakeFiles/opencv_photo.dir/src/denoising.cpp.o" \
"CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o"

# External object files for target opencv_photo
opencv_photo_EXTERNAL_OBJECTS =

lib/libopencv_photo.so.2.4.10: modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o
lib/libopencv_photo.so.2.4.10: modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o
lib/libopencv_photo.so.2.4.10: lib/libopencv_core.so.2.4.10
lib/libopencv_photo.so.2.4.10: lib/libopencv_imgproc.so.2.4.10
lib/libopencv_photo.so.2.4.10: lib/libopencv_core.so.2.4.10
lib/libopencv_photo.so.2.4.10: modules/photo/CMakeFiles/opencv_photo.dir/build.make
lib/libopencv_photo.so.2.4.10: modules/photo/CMakeFiles/opencv_photo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../lib/libopencv_photo.so"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_photo.dir/link.txt --verbose=$(VERBOSE)
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && $(CMAKE_COMMAND) -E cmake_symlink_library ../../lib/libopencv_photo.so.2.4.10 ../../lib/libopencv_photo.so.2.4 ../../lib/libopencv_photo.so

lib/libopencv_photo.so.2.4: lib/libopencv_photo.so.2.4.10

lib/libopencv_photo.so: lib/libopencv_photo.so.2.4.10

# Rule to build all files generated by this target.
modules/photo/CMakeFiles/opencv_photo.dir/build: lib/libopencv_photo.so
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/build

modules/photo/CMakeFiles/opencv_photo.dir/requires: modules/photo/CMakeFiles/opencv_photo.dir/src/denoising.cpp.o.requires
modules/photo/CMakeFiles/opencv_photo.dir/requires: modules/photo/CMakeFiles/opencv_photo.dir/src/inpaint.cpp.o.requires
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/requires

modules/photo/CMakeFiles/opencv_photo.dir/clean:
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo && $(CMAKE_COMMAND) -P CMakeFiles/opencv_photo.dir/cmake_clean.cmake
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/clean

modules/photo/CMakeFiles/opencv_photo.dir/depend:
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/photo/CMakeFiles/opencv_photo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/photo/CMakeFiles/opencv_photo.dir/depend

