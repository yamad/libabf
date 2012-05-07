include_directories(${libabf_SOURCE_DIR}/src)
add_library(stream STATIC stream.c memstream.c ${libabf_SOURCE_DIR}/src/swap.c)


