# cmake -B build -DSOURCE_DIRS="/path/to/src;path/to/tests"
# make format
# make check-format

set(CLANG_FORMAT "${CONDA_PREFIX}/bin/clang-format-14")
set(SOURCE_DIRS "" CACHE STRING "Directories to format, separated by semicolons")

if(EXISTS ${CLANG_FORMAT})
    message(STATUS "Found clang-format at ${CLANG_FORMAT}")

    if(NOT SOURCE_DIRS)
        set(SOURCE_DIRS
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/tests
            ${CMAKE_SOURCE_DIR}/benchmarks
        )
        message(STATUS "Using default source directories: ${SOURCE_DIRS}")
    else()
        message(STATUS "Using user-specified source directories: ${SOURCE_DIRS}")
    endif()

    # split by ;
    string(REPLACE ";" " " SOURCE_DIRS_SPACE "${SOURCE_DIRS}")

    add_custom_target(format
            COMMENT "Running clang-format"
            COMMAND find ${SOURCE_DIRS_SPACE} -name '*.cc' -o -name '*.cpp' -o -name '*.h' | grep -v "_generated.h" | xargs ${CLANG_FORMAT} -i)

    add_custom_target(check-format
            COMMENT "Running clang-format"
            COMMAND find ${SOURCE_DIRS_SPACE} -name '*.cc' -o -name '*.cpp' -o -name '*.h' | grep -v "_generated.h" | xargs ${CLANG_FORMAT} --Werror --dry-run)
else()
    message(FATAL_ERROR "clang-format-14 not found")
endif()
