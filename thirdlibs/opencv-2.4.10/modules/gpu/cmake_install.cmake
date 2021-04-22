# Install script for directory: C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/smartx1.0/modules/opencv/opencv-2.4.10/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/lib/Debug/opencv_gpu2410d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/lib/Release/opencv_gpu2410.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibsx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/bin/Debug/opencv_gpu2410d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/bin/Release/opencv_gpu2410.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/devmem2d.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/gpu.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/gpumat.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/stream_accessor.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/block.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/border_interpolate.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/color.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/common.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/datamov_utils.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/dynamic_smem.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/emulation.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/filters.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/funcattrib.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/functional.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/limits.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/reduce.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/saturate_cast.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/scan.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/simd_functions.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/static_check.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/transform.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/type_traits.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/utility.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/vec_distance.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/vec_math.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/vec_traits.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/warp.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/warp_reduce.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/warp_shuffle.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device/detail" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/detail/color_detail.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device/detail" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/detail/reduce.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device/detail" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/detail/reduce_key_val.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device/detail" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/detail/transform_detail.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device/detail" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/detail/type_traits_detail.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/gpu/device/detail" TYPE FILE FILES "C:/smartx1.0/modules/opencv/opencv-2.4.10/modules/gpu/include/opencv2/gpu/device/detail/vec_distance_detail.hpp")
endif()

