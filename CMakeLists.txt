cmake_minimum_required(VERSION 2.8.4) # >= 2.8.4 needed for CYGWIN behavior
project(libabf C)

include_directories(src)
include_directories(src/platforms/gcc)
add_subdirectory(src)

include_directories(vendor/ceedling/vendor/unity/src)
add_subdirectory(test)
