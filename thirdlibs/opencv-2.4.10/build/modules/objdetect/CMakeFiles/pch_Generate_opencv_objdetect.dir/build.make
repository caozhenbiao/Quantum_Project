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
CMAKE_SOURCE_DIR = /mnt/hgfs/smartx/soft/libs/opencv-2.4.10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build

# Utility rule file for pch_Generate_opencv_objdetect.

modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect: modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch

modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch: ../modules/objdetect/src/precomp.hpp
modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch: modules/objdetect/precomp.hpp
modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch: lib/libopencv_objdetect_pch_dephelp.a
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp.gch/opencv_objdetect_Release.gch"
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect && /usr/bin/cmake -E make_directory /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect/precomp.hpp.gch
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect && /usr/bin/c++ -O3 -DNDEBUG -DNDEBUG -fPIC -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/highgui/include" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/imgproc/include" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/core/include" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/objdetect/src" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/objdetect/include" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build" -fsigned-char -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -msse -msse2 -msse3 -ffunction-sections -DCVAPI_EXPORTS -x c++-header -o /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect/precomp.hpp

modules/objdetect/precomp.hpp: ../modules/objdetect/src/precomp.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp"
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect && /usr/bin/cmake -E copy /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/objdetect/src/precomp.hpp /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect/precomp.hpp

pch_Generate_opencv_objdetect: modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect
pch_Generate_opencv_objdetect: modules/objdetect/precomp.hpp.gch/opencv_objdetect_Release.gch
pch_Generate_opencv_objdetect: modules/objdetect/precomp.hpp
pch_Generate_opencv_objdetect: modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/build.make
.PHONY : pch_Generate_opencv_objdetect

# Rule to build all files generated by this target.
modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/build: pch_Generate_opencv_objdetect
.PHONY : modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/build

modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/clean:
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect && $(CMAKE_COMMAND) -P CMakeFiles/pch_Generate_opencv_objdetect.dir/cmake_clean.cmake
.PHONY : modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/clean

modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/depend:
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/smartx/soft/libs/opencv-2.4.10 /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/objdetect /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/objdetect/CMakeFiles/pch_Generate_opencv_objdetect.dir/depend

