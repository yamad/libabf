include_directories(${libabf_SOURCE_DIR}/src)
include_directories(${libabf_SOURCE_DIR}/src/stream)
include_directories(${libabf_SOURCE_DIR}/vendor/ceedling/vendor/unity/src)

add_subdirectory(support)
include_directories(support)

add_executable(test_MemStream test_MemStream.c)
target_link_libraries(test_MemStream stream streamtest_utils unity)

add_library(unity STATIC ${libabf_SOURCE_DIR}/vendor/ceedling/vendor/unity/src/unity.c)

