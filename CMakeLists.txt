cmake_minimum_required(VERSION 3.24)

project(Ore)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(ENABLE_CCACHE "Use ccache to improve recompilation speed" ON)

if (MSVC)
  message(FATAL_ERROR "Ore only supports gcc and clang currently.")
endif()

set(ON_UNIX OFF)
set(ORE_MODULES_DIR ${CMAKE_INSTALL_LIBDIR}/ore/)

if (UNIX)
  set(ON_UNIX ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (ENABLE_CCACHE)
  find_package(CCache)
  if (CCache_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCache_EXECUTABLE}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK    "${CCache_EXECUTABLE}")
  endif()
endif()

configure_file(Source/Config.h.in Config.h @ONLY)

add_subdirectory(ThirdParty)

set(LIB_SOURCES Source/AST.cpp
            Source/Interpreter.cpp
            Source/Runtime/Value.cpp
            Source/Runtime/Object.cpp
            Source/Runtime/GlobalObject.cpp
            Source/Runtime/GlobalObjectShape.cpp
            Source/Runtime/StringObject.cpp
            Source/Runtime/NumberObject.cpp
            Source/Runtime/NumberObjectShape.cpp
            Source/Runtime/ArrayObject.cpp
            Source/Runtime/ArrayObjectShape.cpp
            Source/Runtime/ExceptionObject.cpp
            Source/GC/Cell.cpp
            Source/GC/Heap.cpp
            Source/GC/HeapBlock.cpp
            Source/Parser/Token.cpp
            Source/Parser/Lexer.cpp
            Source/Parser/RDParser.cpp)

if (ON_UNIX)
  list(APPEND LIB_SOURCES Source/Runtime/FFIObject.cpp)
endif()


file(GLOB_RECURSE ORE_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/Source "Source/*.h")

foreach(header ${ORE_INCLUDE_FILES})
  get_filename_component(subdir ${header} DIRECTORY)
  install(FILES Source/${header} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ore/${subdir}" OPTIONAL)
endforeach()

add_library(ore SHARED ${LIB_SOURCES})
target_include_directories(ore PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(ore PRIVATE fmt)
install(TARGETS ore LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_executable(orepl REPL/main.cpp
              REPL/REPLGlobalObjectShape.cpp)
target_include_directories(orepl PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(orepl PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(orepl PRIVATE ore cxxopts fmt readline)

install(
  TARGETS orepl
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

add_subdirectory(Modules)
