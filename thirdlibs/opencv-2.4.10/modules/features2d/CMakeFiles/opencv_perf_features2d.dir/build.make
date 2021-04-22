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
include modules/features2d/CMakeFiles/opencv_perf_features2d.dir/depend.make

# Include the progress variables for this target.
include modules/features2d/CMakeFiles/opencv_perf_features2d.dir/progress.make

# Include the compile flags for this target's objects.
include modules/features2d/CMakeFiles/opencv_perf_features2d.dir/flags.make

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/flags.make
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o: modules/features2d/perf/perf_batchDistance.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o -c /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_batchDistance.cpp

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.i"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -E /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_batchDistance.cpp > CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.i

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.s"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -S /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_batchDistance.cpp -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.s

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.requires:
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.requires

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.provides: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.requires
	$(MAKE) -f modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build.make modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.provides.build
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.provides

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.provides.build: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.provides.build

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/flags.make
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o: modules/features2d/perf/perf_fast.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o -c /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_fast.cpp

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.i"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -E /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_fast.cpp > CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.i

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.s"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -S /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_fast.cpp -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.s

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.requires:
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.requires

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.provides: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.requires
	$(MAKE) -f modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build.make modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.provides.build
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.provides

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.provides.build: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.provides.build

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/flags.make
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o: modules/features2d/perf/perf_main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o -c /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_main.cpp

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.i"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -E /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_main.cpp > CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.i

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.s"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -S /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_main.cpp -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.s

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.requires:
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.requires

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.provides: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.requires
	$(MAKE) -f modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build.make modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.provides.build
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.provides

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.provides.build: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.provides.build

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/flags.make
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o: modules/features2d/perf/perf_orb.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o -c /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_orb.cpp

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.i"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -E /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_orb.cpp > CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.i

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.s"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS)  -include "/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf_precomp.hpp" -Winvalid-pch  -S /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/perf/perf_orb.cpp -o CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.s

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.requires:
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.requires

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.provides: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.requires
	$(MAKE) -f modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build.make modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.provides.build
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.provides

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.provides.build: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.provides.build

# Object files for target opencv_perf_features2d
opencv_perf_features2d_OBJECTS = \
"CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o" \
"CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o" \
"CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o" \
"CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o"

# External object files for target opencv_perf_features2d
opencv_perf_features2d_EXTERNAL_OBJECTS =

bin/opencv_perf_features2d: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o
bin/opencv_perf_features2d: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o
bin/opencv_perf_features2d: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o
bin/opencv_perf_features2d: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o
bin/opencv_perf_features2d: lib/libopencv_core.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_flann.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_imgproc.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_highgui.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_features2d.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_ts.a
bin/opencv_perf_features2d: lib/libopencv_highgui.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_core.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_flann.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_imgproc.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_highgui.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_features2d.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_video.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_flann.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_highgui.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_imgproc.so.2.4.10
bin/opencv_perf_features2d: lib/libopencv_core.so.2.4.10
bin/opencv_perf_features2d: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build.make
bin/opencv_perf_features2d: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../bin/opencv_perf_features2d"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_perf_features2d.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build: bin/opencv_perf_features2d
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/build

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/requires: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_batchDistance.cpp.o.requires
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/requires: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_fast.cpp.o.requires
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/requires: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_main.cpp.o.requires
modules/features2d/CMakeFiles/opencv_perf_features2d.dir/requires: modules/features2d/CMakeFiles/opencv_perf_features2d.dir/perf/perf_orb.cpp.o.requires
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/requires

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/clean:
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d && $(CMAKE_COMMAND) -P CMakeFiles/opencv_perf_features2d.dir/cmake_clean.cmake
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/clean

modules/features2d/CMakeFiles/opencv_perf_features2d.dir/depend:
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/CMakeFiles/opencv_perf_features2d.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/features2d/CMakeFiles/opencv_perf_features2d.dir/depend

