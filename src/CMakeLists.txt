add_library(abf2 abf2.c deserialize.c swap.c memory.c)

add_subdirectory(stream)
include_directories(stream)

add_library(abf2_stream abf2_stream.c)
target_link_libraries(abf2_stream abf2 stream)

add_library(abf2_print abf2_print.c)
target_link_libraries(abf2_print abf2)

add_executable(abf2_info abf2_info.c)
target_link_libraries(abf2_info abf2_print)