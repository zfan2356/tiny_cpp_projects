# include(CodeCoverage)
# cmake -B build -DENABLE_CODE_COVERAGE=ON
# for GCC
# gcovr -r .. --html --html-details -o coverage.html
# for Clang
# llvm-cov gcov -f -b your_program

option(ENABLE_CODE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_CODE_COVERAGE)
    if(CMAKE_BUILD_TYPE_UC STREQUAL "DEBUG" )
        message(STATUS "Enable code coverage with debug mode.")
    else()
        message(WARNING "Code coverage with no debug mode!!!")
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(COVERAGE_COMPILER_FLAGS "-g -fprofile-arcs -ftest-coverage")
        link_libraries(gcov)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
        set(COVERAGE_COMPILER_FLAGS "-g -fprofile-instr-generate -fcoverage-mapping")
    else()
        message(FATAL_ERROR "Unknown compiler id ${CMAKE_CXX_COMPILER_ID}")
    endif()

    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-fprofile-abs-path HAVE_fprofile_abs_path)
    if(HAVE_fprofile_abs_path)
        set(COVERAGE_COMPILER_FLAGS "${COVERAGE_COMPILER_FLAGS} -fprofile-abs-path")
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}")
    message(STATUS "Appending code coverage compiler flags: ${COVERAGE_COMPILER_FLAGS}")
endif()
