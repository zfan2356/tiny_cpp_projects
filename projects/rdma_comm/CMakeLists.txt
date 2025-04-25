cmake_minimum_required(VERSION 3.12)
project(rdma_comm VERSION 0.1.0 LANGUAGES C CXX)

if(DEFINED CONDA_PREFIX)
    list(PREPEND CMAKE_PREFIX_PATH "${CONDA_PREFIX}")
endif()

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

set(CMAKE_CONFIGURATION_TYPES "RelWithDebInfo;Debug;Release;MinSizeRel" CACHE STRING "" FORCE)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "None")
    set (CMAKE_BUILD_TYPE "RelWithDebInfo")
    message (STATUS "CMAKE_BUILD_TYPE is not set, set to default = ${CMAKE_BUILD_TYPE}")
endif ()

message (STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
string (TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UC)

if (CMAKE_BUILD_TYPE_UC STREQUAL "DEBUG")
    option(ENABLE_ASSERTIONS "Enable assertions" ON)
else ()
    option(ENABLE_ASSERTIONS "Enable assertions" OFF)
endif ()

message(STATUS "ENABLE_ASSERTIONS: ${ENABLE_ASSERTIONS}")

# -UNDEBUG can be used to disable assertions in non-debug builds
if(ENABLE_ASSERTIONS)
    add_definitions(-D_DEBUG)
    # On non-Debug builds cmake automatically defines NDEBUG, so we explicitly undefine it:
else()
    # NOTE: use `add_compile_options` rather than `add_definitions` since
    # `add_definitions` does not support generator expressions.
    add_compile_options($<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:-UNDEBUG>)
endif()

option(OVERRIDE_CXX_NEW_DELETE "Override C++ new/delete operator" OFF)
option(SAVE_ALLOCATE_SIZE "Use more memory to save allocate size" OFF)

option(ENABLE_FUSE_APPLICATION "" ON)

if (DEFINED SANITIZER AND SANITIZER)
    set(OVERRIDE_CXX_NEW_DELETE OFF)
endif()
message (STATUS "OVERRIDE_CXX_NEW_DELETE: ${OVERRIDE_CXX_NEW_DELETE}")

if (OVERRIDE_CXX_NEW_DELETE)
    add_definitions(-DOVERRIDE_CXX_NEW_DELETE)
    if (SAVE_ALLOCATE_SIZE)
        add_definitions(-DSAVE_ALLOCATE_SIZE)
    endif()
endif()
message (STATUS "SAVE_ALLOCATE_SIZE: ${SAVE_ALLOCATE_SIZE}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Clang compiler detected")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
    add_link_options(-fuse-ld=lld)
    # Do not build with libc++ (LLVM's implementation of the C++ standard library) in fdb
    set(USE_LIBCXX OFF)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "GCC compiler detected")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
endif()

# Remove project root from the __FILE__ macro variable
add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    add_compile_options(-msse4.2 -mavx2)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    # ARM architecture detected:
    # Clang on ARM uses built-in runtime library (compiler-rt) to provide symbols like '__muloti4'.
    # Without explicitly specifying '-rtlib=compiler-rt', linking may fail due to missing '__muloti4'.
    # '-unwindlib=libgcc' ensures proper exception unwinding compatibility.
    add_compile_options(-march=armv8-a)
    message(STATUS "ARM architecture detected, linking with compiler-rt and libgcc.")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rtlib=compiler-rt -unwindlib=libgcc")
endif()

# include some common cmake files
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)
include(Sanitizers)
include(CompileFlags)

store_compile_flags()
# Define THIRD_PARTY_DIR as the /3rd directory in the parent of PROJECT_SOURCE_DIR
get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
set(THIRD_PARTY_DIR "${PARENT_DIR}/3rd")
message(STATUS "Third party directory: ${THIRD_PARTY_DIR}")

add_subdirectory("${THIRD_PARTY_DIR}/fmt" "${CMAKE_BINARY_DIR}/fmt_build" EXCLUDE_FROM_ALL)
restore_compile_flags()
set(ZSTD_BUILD_STATIC ON)
# Add zstd with explicit binary directory
add_subdirectory("${THIRD_PARTY_DIR}/zstd/build/cmake" "${CMAKE_BINARY_DIR}/zstd_build" EXCLUDE_FROM_ALL)
set(zstd_INCLUDE_DIRS "${THIRD_PARTY_DIR}/zstd/lib")
set(ZSTD_INCLUDE_DIR "${THIRD_PARTY_DIR}/zstd/lib")
set(zstd_FOUND ON)
set(ZSTD_FOUND ON)
set(zstd_LIBRARIES "${CMAKE_BINARY_DIR}/zstd_build/lib/libzstd.a")
set(ZSTD_LIBRARY "${CMAKE_BINARY_DIR}/zstd_build/lib/libzstd.a")
restore_compile_flags()
add_subdirectory("${THIRD_PARTY_DIR}/googletest" "${CMAKE_BINARY_DIR}/googletest_build" EXCLUDE_FROM_ALL)
restore_compile_flags()
set(FOLLY_NO_EXCEPTION_TRACER ON)
add_subdirectory("${THIRD_PARTY_DIR}/folly" "${CMAKE_BINARY_DIR}/folly_build" EXCLUDE_FROM_ALL)
restore_compile_flags()
add_subdirectory("${THIRD_PARTY_DIR}/pybind11" "${CMAKE_BINARY_DIR}/pybind11_build" EXCLUDE_FROM_ALL)
restore_compile_flags()
add_subdirectory("${THIRD_PARTY_DIR}/scnlib" "${CMAKE_BINARY_DIR}/scnlib_build" EXCLUDE_FROM_ALL)
restore_compile_flags()
add_subdirectory("${THIRD_PARTY_DIR}/toml11" "${CMAKE_BINARY_DIR}/toml11_build" EXCLUDE_FROM_ALL)
restore_compile_flags()
set (MI_OVERRIDE OFF)
add_subdirectory("${THIRD_PARTY_DIR}/mimalloc" "${CMAKE_BINARY_DIR}/mimalloc_build" EXCLUDE_FROM_ALL)
restore_compile_flags()

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

# set(Boost_USE_STATIC_LIBS ON)
set(FDB_VERSION 7.1.5-ibe)

find_package(Threads REQUIRED)
# find_package(Boost REQUIRED COMPONENTS filesystem system program_options)
find_library(LIBUV_LIBRARY NAMES libuv1)
enable_testing()

# code coverage
include(CodeCoverage)
# clang format
set(SOURCE_DIRS "${CMAKE_SOURCE_DIR}/csrc;${CMAKE_SOURCE_DIR}/tests;${CMAKE_SOURCE_DIR}/benchmarks")
include(CLangFormat)
# clang tidy
set(CLANG_TIDY_SOURCE_DIRS "${CMAKE_SOURCE_DIR}/csrc;${CMAKE_SOURCE_DIR}/tests;${CMAKE_SOURCE_DIR}/benchmarks")
include(CLangTidy)
# target
include(Target)
# # dump config
# include(DumpConfig)
# # jemalloc
# include(Jemalloc)
# # apache arrow
# include(ApacheArrow)

# configure_file(CTestCustom.cmake ${CMAKE_BINARY_DIR} @ONLY)

add_subdirectory(csrc)
add_subdirectory(tests)
