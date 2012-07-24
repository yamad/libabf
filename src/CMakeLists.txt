include_directories(lib)
include_directories(serialize)
include_directories(stream)
include_directories(abf)
include_directories(abf2)

# general libraries
add_library(memory lib/memory.c)
add_library(printutil lib/printutil.c)

# serialization
add_library(deserialize serialize/deserialize.c serialize/swap.c)
target_link_libraries(deserialize memory)

# streams
add_library(stream STATIC stream/stream.c stream/memstream.c)
target_link_libraries(stream deserialize)

# ABF general
add_library(abf_string abf/abf_string.c)

add_library(abf_read abf/abf_read.c)
target_link_libraries(abf_read abf_string deserialize)

add_library(abf_print abf/abf_print.c)
target_link_libraries(abf_print abf_read printutil)

# ABF2
add_library(abf2_read abf2/abf2_read.c)
target_link_libraries(abf2_read deserialize)

add_library(abf2_print abf2/abf2_print.c)
target_link_libraries(abf2_print abf2_read printutil)

add_library(abf2_stream abf2/abf2_stream.c)
target_link_libraries(abf2_stream abf2_read stream)

add_executable(abf2_info abf2/abf2_info.c)
target_link_libraries(abf2_info abf2_print abf_print abf_read)