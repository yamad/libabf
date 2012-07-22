add_library(deserialize deserialize.c swap.c memory.c)

add_library(abf2_read abf2_read.c)
target_link_libraries(abf2_read deserialize)

add_library(abf_read abf_read.c)
target_link_libraries(abf_read deserialize)

add_library(printutil printutil.c)

add_library(abf2_print abf2_print.c)
target_link_libraries(abf2_print abf2_read printutil)

add_library(abf_print abf_print.c)
target_link_libraries(abf_print abf_read printutil)

add_executable(abf2_info abf2_info.c)
target_link_libraries(abf2_info abf2_print abf_print abf_read)

add_subdirectory(stream)
include_directories(stream)

add_library(abf2_stream abf2_stream.c)
target_link_libraries(abf2_stream abf2_read stream)
