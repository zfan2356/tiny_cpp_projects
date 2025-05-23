cmake_minimum_required(VERSION 3.12)
project(webserver VERSION 0.1.0 LANGUAGES CXX)

if(DEFINED CONDA_PREFIX)
    list(PREPEND CMAKE_PREFIX_PATH "${CONDA_PREFIX}")
    # set(CMAKE_CXX_COMPILER "${CONDA_PREFIX}/bin/clang++")
    # set(CMAKE_C_COMPILER "${CONDA_PREFIX}/bin/clang")
endif()

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
set(THIRD_PARTY_DIR "${PARENT_DIR}/3rd")
message(STATUS "Third party directory: ${THIRD_PARTY_DIR}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     message(STATUS "Clang compiler detected")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
#     set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
#     add_link_options(-fuse-ld=lld)
#     # Do not build with libc++ (LLVM's implementation of the C++ standard library) in fdb
#     set(USE_LIBCXX OFF)
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#     message(STATUS "GCC compiler detected")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
# endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

set (CMAKE_CXX_FLAGS                     "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO      "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 ${DEBUG_INFO_FLAGS} ${CMAKE_CXX_FLAGS_ADD}")
set (CMAKE_CXX_FLAGS_DEBUG               "${CMAKE_CXX_FLAGS_DEBUG} -O0 ${DEBUG_INFO_FLAGS} -fno-inline ${CMAKE_CXX_FLAGS_ADD}")

set (CMAKE_C_FLAGS                       "${CMAKE_C_FLAGS} ${COMPILER_FLAGS} ${CMAKE_C_FLAGS_ADD}")
set (CMAKE_C_FLAGS_RELWITHDEBINFO        "${CMAKE_C_FLAGS_RELWITHDEBINFO} -O3 ${DEBUG_INFO_FLAGS} ${CMAKE_C_FLAGS_ADD}")
set (CMAKE_C_FLAGS_DEBUG                 "${CMAKE_C_FLAGS_DEBUG} -O0 ${DEBUG_INFO_FLAGS} -fno-inline ${CMAKE_C_FLAGS_ADD}")

set (CMAKE_ASM_FLAGS                     "${CMAKE_ASM_FLAGS} ${COMPILER_FLAGS} ${CMAKE_ASM_FLAGS_ADD}")
set (CMAKE_ASM_FLAGS_RELWITHDEBINFO      "${CMAKE_ASM_FLAGS_RELWITHDEBINFO} -O3 ${DEBUG_INFO_FLAGS} ${CMAKE_ASM_FLAGS_ADD}")
set (CMAKE_ASM_FLAGS_DEBUG               "${CMAKE_ASM_FLAGS_DEBUG} -O0 ${DEBUG_INFO_FLAGS} -fno-inline ${CMAKE_ASM_FLAGS_ADD}")

include(Target)
include(CompileFlags)

store_compile_flags()

add_subdirectory(src)
add_subdirectory(tests)
