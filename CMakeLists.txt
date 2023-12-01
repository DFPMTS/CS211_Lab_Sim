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
    src/Cache.cpp
)

add_executable(
    CacheSim 
    src/MainCache.cpp 
    src/MemoryManager.cpp 
    src/Cache.cpp
)

add_executable(
    CacheOptimized
    src/MainCacheOptimization.cpp
    src/MemoryManager.cpp
    src/Cache.cpp
)

add_executable(
    Proj2
    src/Project2.cpp
    src/MemoryManager.cpp
    src/CacheLayer.cpp
    src/L1L2Cache.cpp
)   

add_executable(ToDirenoTrace src/ToDirenoTrace.cpp)