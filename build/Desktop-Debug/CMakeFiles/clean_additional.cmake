# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appFridge_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appFridge_autogen.dir/ParseCache.txt"
  "CMakeFiles/testFridge_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/testFridge_autogen.dir/ParseCache.txt"
  "appFridge_autogen"
  "testFridge_autogen"
  )
endif()
