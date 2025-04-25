# clang-tidy generates too many warnings, so just disable it by default.
# cmake -B build -DENABLE_CLANG_TIDY=ON
# make clang-tidy
# make clang-tidy-fix

option(ENABLE_CLANG_TIDY "Run clang-tidy during build" OFF)

set(CLANG_TIDY_SOURCE_DIRS "" CACHE STRING "Directories to analyze with clang-tidy, separated by semicolons")

find_program(CLANG_TIDY NAMES clang-tidy-14)
if(CLANG_TIDY)
    if(CMake_SOURCE_DIR STREQUAL CMake_BINARY_DIR)
        message(FATAL_ERROR "CMake_RUN_CLANG_TIDY requires an out-of-source build!")
    endif()

    if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
        message(WARNING "CMAKE_EXPORT_COMPILE_COMMANDS=OFF, clang-tidy may not works!!!")
    endif()

    if(NOT CLANG_TIDY_SOURCE_DIRS)
        set(CLANG_TIDY_SOURCE_DIRS
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/tests
            ${CMAKE_SOURCE_DIR}/demos
            ${CMAKE_SOURCE_DIR}/benchmarks
        )
        message(STATUS "Using default clang-tidy source directories: ${CLANG_TIDY_SOURCE_DIRS}")
    else()
        message(STATUS "Using user-specified clang-tidy source directories: ${CLANG_TIDY_SOURCE_DIRS}")
    endif()

    string(REPLACE ";" " " SOURCE_DIRS_SPACE "${CLANG_TIDY_SOURCE_DIRS}")

    string(REPLACE ";" "\\|" HEADER_FILTER_PATTERN "${CLANG_TIDY_SOURCE_DIRS}")
    set(HEADER_FILTER "${HEADER_FILTER_PATTERN}")

    if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY} --header-filter ${HEADER_FILTER})

        # Create a preprocessor definition that depends on .clang-tidy content
        file(SHA1 ${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy clang_tidy_sha1)
        set(CLANG_TIDY_DEFINITIONS "CLANG_TIDY_SHA1=${clang_tidy_sha1}")
        unset(clang_tidy_sha1)

        configure_file(.clang-tidy .clang-tidy COPYONLY)
    endif()

    add_custom_target(clang-tidy
        COMMENT "Running clang-tidy"
        COMMAND run-clang-tidy-14 -header-filter ${HEADER_FILTER} `find ${SOURCE_DIRS_SPACE} -name "*.cc" -o -name "*.cpp" -not -name "*.actor.cpp" ` -quiet)

    add_custom_target(clang-tidy-fix
        COMMENT "Running clang-tidy -fix"
        COMMAND run-clang-tidy-14 -header-filter ${HEADER_FILTER} `find ${SOURCE_DIRS_SPACE} -name "*.cc" -o -name "*.cpp" -not -name "*.actor.cpp" ` -fix -quiet)
else()
    message(WARNING "clang-tidy-14 not found!!!")
endif()
