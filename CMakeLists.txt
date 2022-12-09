cmake_minimum_required(VERSION 3.24)

project(Ore)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)


set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fsanitize=address")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCES src/AST.cpp
            src/Interpreter.cpp
            src/Runtime/Value.cpp
            src/Runtime/Object.cpp
            src/Runtime/GlobalObject.cpp
            src/Runtime/StringObject.cpp
            src/Runtime/ArrayObject.cpp
            src/GC/Cell.cpp
            src/GC/Heap.cpp
            src/GC/HeapBlock.cpp)

add_library(ore SHARED ${LIB_SOURCES})
target_compile_features(ore PRIVATE cxx_std_17)

add_executable(ore_test tests/main.cpp)
target_include_directories(ore_test PRIVATE src/)
target_link_libraries(ore_test ore)
