cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

if (PASTAPI_BUILD_EXAMPLE_PLUGIN)
  add_subdirectory(example)
endif ()

add_subdirectory(logic)
add_subdirectory(values)
add_subdirectory(timers)
add_subdirectory(math)
add_subdirectory(opencv)
add_subdirectory(filesystem)
