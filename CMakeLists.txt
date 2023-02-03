cmake_minimum_required(VERSION 3.24)

project(Ore)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)


set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCES Source/AST.cpp
            Source/Interpreter.cpp
            Source/Runtime/Value.cpp
            Source/Runtime/Object.cpp
            Source/Runtime/GlobalObject.cpp
            Source/Runtime/GlobalObjectShape.cpp
            Source/Runtime/StringObject.cpp
            Source/Runtime/ArrayObject.cpp
            Source/Runtime/FFIObject.cpp
            Source/GC/Cell.cpp
            Source/GC/Heap.cpp
            Source/GC/HeapBlock.cpp)

add_library(ore SHARED ${LIB_SOURCES})
target_compile_features(ore PRIVATE cxx_std_20)

add_executable(ore_test Tests/main.cpp)
target_include_directories(ore_test PRIVATE Source/)
target_link_libraries(ore_test ore)

add_library(ffi_test SHARED Tests/FFI_tests/main.cpp)
target_include_directories(ffi_test PRIVATE Source/)
