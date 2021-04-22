# -----------------------------------------------
# File that provides "make uninstall" target
#  We use the file 'install_manifest.txt'
# -----------------------------------------------
IF(NOT EXISTS "C:/smartx1.0/modules/opencv/opencv-2.4.10/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"C:/smartx1.0/modules/opencv/opencv-2.4.10/install_manifest.txt\"")
ENDIF(NOT EXISTS "C:/smartx1.0/modules/opencv/opencv-2.4.10/install_manifest.txt")

FILE(READ "C:/smartx1.0/modules/opencv/opencv-2.4.10/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  IF(EXISTS "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM(
      "C:/myprograms/cmake-3.12.0-win64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    IF(NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    ENDIF(NOT "${rm_retval}" STREQUAL 0)
  ELSE(EXISTS "$ENV{DESTDIR}${file}")
    MESSAGE(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  ENDIF(EXISTS "$ENV{DESTDIR}${file}")
ENDFOREACH(file)
