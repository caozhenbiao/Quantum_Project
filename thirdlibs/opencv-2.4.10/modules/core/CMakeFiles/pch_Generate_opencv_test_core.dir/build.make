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

# Utility rule file for pch_Generate_opencv_test_core.

modules/core/CMakeFiles/pch_Generate_opencv_test_core: modules/core/test_precomp.hpp.gch/opencv_test_core_Release.gch

modules/core/test_precomp.hpp.gch/opencv_test_core_Release.gch: modules/core/test/test_precomp.hpp
modules/core/test_precomp.hpp.gch/opencv_test_core_Release.gch: modules/core/test_precomp.hpp
modules/core/test_precomp.hpp.gch/opencv_test_core_Release.gch: lib/libopencv_test_core_pch_dephelp.a
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating test_precomp.hpp.gch/opencv_test_core_Release.gch"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core && /usr/bin/cmake -E make_directory /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/test_precomp.hpp.gch
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core && /usr/bin/c++ -O2 -DNDEBUG -DNDEBUG -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/test" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/video/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/features2d/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/highgui/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/imgproc/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/flann/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/ts/include" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/src" -isystem"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/3rdparty/zlib" -I"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/dynamicuda/include" -isystem"/mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10" -fsigned-char -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wno-narrowing -Wno-delete-non-virtual-dtor -Wno-unnamed-type-template-args -fdiagnostics-show-option -pthread -march=i686 -fomit-frame-pointer -msse -msse2 -msse3 -mfpmath=sse -ffunction-sections -x c++-header -o /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/test_precomp.hpp.gch/opencv_test_core_Release.gch /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/test_precomp.hpp

modules/core/test_precomp.hpp: modules/core/test/test_precomp.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating test_precomp.hpp"
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core && /usr/bin/cmake -E copy /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/test/test_precomp.hpp /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/test_precomp.hpp

pch_Generate_opencv_test_core: modules/core/CMakeFiles/pch_Generate_opencv_test_core
pch_Generate_opencv_test_core: modules/core/test_precomp.hpp.gch/opencv_test_core_Release.gch
pch_Generate_opencv_test_core: modules/core/test_precomp.hpp
pch_Generate_opencv_test_core: modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/build.make
.PHONY : pch_Generate_opencv_test_core

# Rule to build all files generated by this target.
modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/build: pch_Generate_opencv_test_core
.PHONY : modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/build

modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/clean:
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core && $(CMAKE_COMMAND) -P CMakeFiles/pch_Generate_opencv_test_core.dir/cmake_clean.cmake
.PHONY : modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/clean

modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/depend:
	cd /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10 /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core /mnt/hgfs/smartx1.0/modules/opencv/opencv-2.4.10/modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/core/CMakeFiles/pch_Generate_opencv_test_core.dir/depend

