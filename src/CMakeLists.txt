cmake_minimum_required(VERSION 3.10)
project(IEEEToPosit LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti -fPIC")

SET(NRSSL_PATH "${CMAKE_SOURCE_DIR}/jars")

# Find LLVM
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Find JNI
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# Add the LLVM pass
add_library(IEEEToPosit MODULE IEEEToPositPass.cpp NRSSL.cpp)

# Generate LLVM IR from C file
add_custom_command(
    OUTPUT sample.ll
    COMMAND clang -S -emit-llvm ${CMAKE_SOURCE_DIR}/test/sample.c -o sample.ll
    DEPENDS ${CMAKE_SOURCE_DIR}/test/sample.c
    COMMENT "Generating LLVM IR from sample.c"
)

# Run LLVM pass on sample.ll
add_custom_command(
    OUTPUT processed.ll
    COMMAND "NRSSL_JARS=${NRSSL_PATH}"
            "LD_PRELOAD=$ENV{JAVA_HOME}/lib/server/libjvm.so"
            "opt"
            "-load-pass-plugin=${CMAKE_CURRENT_BINARY_DIR}/libIEEEToPosit.so"
            "-passes=ieee-to-posit"
            "-S" "sample.ll"
            "-o" "processed.ll"
    DEPENDS sample.ll IEEEToPosit
    COMMENT "Running LLVM pass on sample.ll"
)

# Compile processed.ll to an object file
add_custom_command(
    OUTPUT processed.o
    COMMAND llc -filetype=obj --relocation-model=pic processed.ll -o processed.o
    DEPENDS processed.ll
    COMMENT "Compiling processed.ll to object file"
)

# Create the final executable
add_executable(TestPass TestPass.cpp NRSSL.cpp processed.o)

set_target_properties(TestPass PROPERTIES COMPILE_FLAGS "-fPIE")
set_target_properties(TestPass PROPERTIES LINK_FLAGS "-fPIE -pie")

target_link_libraries(TestPass ${JNI_LIBRARIES})
