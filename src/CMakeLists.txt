cmake_minimum_required(VERSION 3.10)
project(IEEEToPosit LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Set LLVM paths
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find JNI
find_package(JNI REQUIRED)

# Set JNI paths
include_directories(${JNI_INCLUDE_DIRS})

# Add the pass
add_library(IEEEToPosit MODULE IEEEToPositPass.cpp NRSSL.cpp)
