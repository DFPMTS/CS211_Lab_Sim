cmake_minimum_required(VERSION 3.1)

project(RISCV-Simulator)

set(CMAKE_CPP_COMPILER clang)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
    Simulator 
    src/MainCPU.cpp 
    src/MemoryManager.cpp 
    src/Simulator.cpp 
    src/BranchPredictor.cpp 
    src/L1L2Cache.cpp
    src/CacheLayer.cpp
)

add_executable(
    Sanity
    src/Sanity.cpp
    src/MemoryManager.cpp
    src/CacheLayer.cpp
    src/L1L2Cache.cpp
)   

add_executable(
    Section1
    src/Section1.cpp
    src/MemoryManager.cpp
    src/CacheLayer.cpp
    src/L1L2Cache.cpp
)   

add_executable(
    Section2
    src/Section2.cpp
    src/MemoryManager.cpp
    src/CacheLayer.cpp
    src/L1L2Cache.cpp
)   

add_executable(
    Section3
    src/Section3.cpp
    src/MemoryManager.cpp
    src/CacheLayer.cpp
    src/L1L2Cache.cpp
)   


add_executable(ToDirenoTrace src/ToDirenoTrace.cpp)