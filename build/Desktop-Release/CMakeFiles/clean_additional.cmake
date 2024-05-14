# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles/MarchingCubesQT_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MarchingCubesQT_autogen.dir/ParseCache.txt"
  "MarchingCubesQT_autogen"
  )
endif()
