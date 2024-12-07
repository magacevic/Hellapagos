# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/hellapagos_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/hellapagos_autogen.dir/ParseCache.txt"
  "hellapagos_autogen"
  )
endif()
