file(GLOB ORE_LIB_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach(LIB_SOURCE ${ORE_LIB_SOURCES})
  get_filename_component(LIB_NAME ${LIB_SOURCE} NAME_WE)

  add_library(${LIB_NAME} SHARED ${LIB_SOURCE})
  target_link_libraries(${LIB_NAME} PRIVATE ore)
  target_include_directories(${LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Source")
  target_include_directories(${LIB_NAME} PRIVATE "${CMAKE_BINARY_DIR}")

  install(TARGETS ${LIB_NAME} LIBRARY DESTINATION ${ORE_MODULES_DIR})
endforeach()
