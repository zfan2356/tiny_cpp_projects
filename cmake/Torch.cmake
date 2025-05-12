find_package(Python COMPONENTS Interpreter REQUIRED)

execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "import torch; print(torch.utils.cmake_prefix_path)"
    OUTPUT_VARIABLE TORCH_CMAKE_PATHS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE TORCH_QUERY_RESULT
)

if(TORCH_QUERY_RESULT EQUAL 0)
    list(GET TORCH_CMAKE_PATHS 0 TORCH_CMAKE_PATH)
else()
    set(TORCH_CMAKE_PATH "$ENV{TORCH_DIR}")
endif()

if(EXISTS "${TORCH_CMAKE_PATH}/Torch/TorchConfig.cmake")
    set(Torch_DIR "${TORCH_CMAKE_PATH}/Torch")
    message(STATUS "Found Torch: ${Torch_DIR}/Torch")
else()
    message(FATAL_ERROR "Torch not found. Options:\n"
        "1. Install PyTorch with pip/conda\n"
        "2. Set TORCH_DIR to libtorch's cmake path\n"
        "3. Use -DTorch_DIR=... in cmake command")
endif()

find_package(Torch REQUIRED)
