# Install script for directory: /mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10"
           RPATH "/usr/local/lib")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
      "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib/libopencv_stitching.so.2.4.10"
      "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib/libopencv_stitching.so.2.4"
      "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib/libopencv_stitching.so"
      )
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10"
           OLD_RPATH "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib"
           NEW_RPATH "/usr/local/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10"
           RPATH "/usr/local/lib")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
      "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib/libopencv_stitching.so.2.4.10"
      "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib/libopencv_stitching.so.2.4"
      "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib/libopencv_stitching.so"
      )
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10"
           OLD_RPATH "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/build/lib"
           NEW_RPATH "/usr/local/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libopencv_stitching.so.2.4.10")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "libs")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/stitcher.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/warpers.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/autocalib.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/blenders.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/camera.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/exposure_compensate.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/matchers.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/motion_estimators.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/seam_finders.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/util.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/util_inl.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/warpers.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2/stitching/detail" TYPE FILE FILES "/mnt/hgfs/smartx/soft/libs/opencv-2.4.10/modules/stitching/include/opencv2/stitching/detail/warpers_inl.hpp")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev")

