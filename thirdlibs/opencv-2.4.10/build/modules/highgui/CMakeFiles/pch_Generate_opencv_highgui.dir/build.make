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

# Utility rule file for pch_Generate_opencv_highgui.

modules/highgui/CMakeFiles/pch_Generate_opencv_highgui: modules/highgui/precomp.hpp.gch/opencv_highgui_Release.gch

modules/highgui/precomp.hpp.gch/opencv_highgui_Release.gch: ../modules/highgui/src/precomp.hpp
modules/highgui/precomp.hpp.gch/opencv_highgui_Release.gch: modules/highgui/precomp.hpp
modules/highgui/precomp.hpp.gch/opencv_highgui_Release.gch: lib/libopencv_highgui_pch_dephelp.a
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp.gch/opencv_highgui_Release.gch"
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui && /usr/bin/cmake -E make_directory /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui/precomp.hpp.gch
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui && /usr/bin/c++ -O3 -DNDEBUG -DNDEBUG -fPIC -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/imgproc/include" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/core/include" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/highgui/src" -I"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/highgui/include" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/libjasper" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/3rdparty/libtiff" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/libtiff" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/libpng" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/libjpeg" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/3rdparty/zlib" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/zlib" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/openexr/Half" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/openexr/Iex" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/openexr/IlmThread" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/openexr/Imath" -isystem"/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/3rdparty/openexr/IlmImf" -DHIGHGUI_EXPORTS -fsigned-char -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -msse -msse2 -msse3 -ffunction-sections -DCVAPI_EXPORTS -x c++-header -o /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui/precomp.hpp.gch/opencv_highgui_Release.gch /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui/precomp.hpp

modules/highgui/precomp.hpp: ../modules/highgui/src/precomp.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp"
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui && /usr/bin/cmake -E copy /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/highgui/src/precomp.hpp /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui/precomp.hpp

pch_Generate_opencv_highgui: modules/highgui/CMakeFiles/pch_Generate_opencv_highgui
pch_Generate_opencv_highgui: modules/highgui/precomp.hpp.gch/opencv_highgui_Release.gch
pch_Generate_opencv_highgui: modules/highgui/precomp.hpp
pch_Generate_opencv_highgui: modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/build.make
.PHONY : pch_Generate_opencv_highgui

# Rule to build all files generated by this target.
modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/build: pch_Generate_opencv_highgui
.PHONY : modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/build

modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/clean:
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui && $(CMAKE_COMMAND) -P CMakeFiles/pch_Generate_opencv_highgui.dir/cmake_clean.cmake
.PHONY : modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/clean

modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/depend:
	cd /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/smartx/soft/libs/opencv-2.4.10 /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/highgui /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/highgui/CMakeFiles/pch_Generate_opencv_highgui.dir/depend
