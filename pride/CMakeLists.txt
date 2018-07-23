
file(GLOB_RECURSE pride_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
sf_create_interface_library(pride SOURCE_LIST ${pride_sources} CXX_VERSION 17)
target_include_directories(${PRIDE_LIB} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
)
