include_directories(${libabf_SOURCE_DIR}/src)
include_directories(${libabf_SOURCE_DIR}/src/lib)
include_directories(${libabf_SOURCE_DIR}/src/serialize)
include_directories(${libabf_SOURCE_DIR}/src/stream)
include_directories(${libabf_SOURCE_DIR}/src/abf)
include_directories(${libabf_SOURCE_DIR}/src/abf2)

include_directories(support)
add_library(streamtest_utils STATIC support/streamtest_utils.c)

set(UNITY_ROOT_DIR ${libabf_SOURCE_DIR}/vendor/ceedling/vendor/unity)
set(UNITY_TEST_GENERATOR ${UNITY_ROOT_DIR}/auto/generate_test_runner.rb)
include_directories(${UNITY_ROOT_DIR}/src)
add_library(unity STATIC ${UNITY_ROOT_DIR}/src/unity.c)
add_definitions(-DUNITY_SUPPORT_64)



# do_build_test -- generate test target from a unity test file
#   `test_src` is the name of the test file (including .c extension)
#    extra arguments are interpreted as library dependencies
#    e.g. do_build_test(test_This.c foo bar)
function(do_build_test test_src)
    string(REPLACE ".c" "" test_src_name ${test_src})
    set(runner_src "${test_src_name}_runner.c")
    add_custom_command(
        OUTPUT ${runner_src}
        COMMAND ruby ${UNITY_TEST_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/${test_src} ${runner_src}
        DEPENDS ${test_src}
        IMPLICIT_DEPENDS
        COMMENT "Building ${test_src_name} test runner..."
    )
    add_executable(${test_src_name} EXCLUDE_FROM_ALL ${test_src} ${runner_src})
    add_custom_command(
        TARGET ${test_src_name}
        POST_BUILD
        COMMAND ./${CMAKE_CURRENT_BUILD_DIR}/${test_src_name}
        COMMENT "Running ${test_src_name} test..."
    )
    set(deps)
    foreach(d ${ARGN})
        list(APPEND deps ${d})
    endforeach()
    target_link_libraries(${test_src_name} ${deps} unity)
endfunction()

# do_add_test -- add test target from a unity test file
set(ALL_TESTS)
macro(do_add_test test_src)
    do_build_test(${test_src} ${ARGN})
    string(REPLACE ".c" "" test_src_name ${test_src})
    list(APPEND ALL_TESTS ${test_src_name})
endmacro()

set(STREAMTEST_DEPS streamtest_utils stream deserialize)
do_add_test(test_memstream.c ${STREAMTEST_DEPS})
do_add_test(test_memstream_creation.c ${STREAMTEST_DEPS})
do_add_test(test_stream.c ${STREAMTEST_DEPS})

message(${ALL_TESTS})
add_custom_target(unity_tests ALL
                  DEPENDS ${ALL_TESTS})
