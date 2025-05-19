enable_testing()
include(GoogleTest)

add_subdirectory("${THIRD_PARTY_DIR}/googletest" "${CMAKE_BINARY_DIR}/googletest_build" EXCLUDE_FROM_ALL)
restore_compile_flags()

target_add_google_test(test_logger
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/test_logger.cc
)

target_add_google_test(test_cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/normal/test_cpp.cc
)

target_add_google_test(test_socket
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/test_socket.cc
    socket
)

target_add_google_test(test_epoll
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/test_epoll.cc
    epoll socket
)
