add_library(abf2 abf2.c deserialize.c swap.c memory.c)

add_subdirectory(stream)
include_directories(stream)

add_library(abf2_stream abf2_stream.c)
target_link_libraries(abf2_stream abf2 stream)

add_executable(abf2_printfileinfo abf2_printfileinfo.c)
target_link_libraries(abf2_printfileinfo abf2)

add_executable(abf2_printprotocolinfo abf2_printprotocolinfo.c)
target_link_libraries(abf2_printprotocolinfo abf2)