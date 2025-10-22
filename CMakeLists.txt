cmake_minimum_required(VERSION 3.20)
project(ILM)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${SRC_DIR}/include)

# Source files
set(SOURCES
    src/ComputeDeltaH.cpp
    src/DiffractionLoss.cpp
    src/FindHorizons.cpp
    src/FreeSpaceLoss.cpp
    src/FresnelIntegral.cpp
    src/InitializeArea.cpp
    src/InitializePointToPoint.cpp
    src/InverseComplementaryCumulativeDistributionFunction.cpp
    src/KnifeEdgeDiffraction.cpp
    src/LinearLeastSquaresFit.cpp
    src/LineOfSightLoss.cpp
    src/LongleyRice.cpp
    src/QuickPfl.cpp
    src/SigmaHFunction.cpp
    src/SmoothSphereDiffraction.cpp
    src/TerrainRoughness.cpp
    src/ValidateInputs.cpp
    src/Variability.cpp
    src/ilm.cpp
    src/ilm_area.cpp
    src/ilm_p2p.cpp
)

# Add the shared library target (DLL on Windows, .so on Linux, .dylib on macOS)
add_library(ILM SHARED ${SOURCES})

# Include the headers for the shared library
target_include_directories(ILM PUBLIC ${INCLUDE_DIR})

# Optionally define export macros (useful on Windows)
target_compile_definitions(ILM PRIVATE ILM_EXPORTS)

# Set ILM_DLL_EXPORT only when building the DLL
if(MSVC)  # Check if using Visual Studio
    target_compile_definitions(ILM PRIVATE ILM_DLL_EXPORT)
    target_compile_options(ILM PRIVATE /W4 /permissive-)
else()
    target_compile_options(ILM PRIVATE -Wall -Wextra -Wpedantic)
endif()


# ---- GoogleTest Integration ----

# Fetch GoogleTest via CMake's FetchContent module
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2  # Use a specific release or latest commit
)

FetchContent_MakeAvailable(googletest)

# --- Tests ---
include(CTest)              # enables testing
include(GoogleTest)         # gtest helpers

# Add the executable for the test suite
add_executable(ILMTest tests/test_main.cpp)

# Link the test executable with GoogleTest and the shared library
target_link_libraries(ILMTest PRIVATE GTest::gtest GTest::gtest_main ILM)

# Auto-discover TEST(...) in the binary produced for each config
gtest_discover_tests(ILMTest)

# Make CTest run the right exe path for multi-config generators (MSVC)
# (gtest_discover_tests handles this; if you ever use add_test manually, prefer $<TARGET_FILE:ILMTest>)