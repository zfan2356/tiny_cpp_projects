enable_testing()
include(GoogleTest)

add_subdirectory("${THIRD_PARTY_DIR}/googletest" "${CMAKE_BINARY_DIR}/googletest_build" EXCLUDE_FROM_ALL)
restore_compile_flags()

target_add_google_test(test_webserver
    utils/test_logger.cc
)
