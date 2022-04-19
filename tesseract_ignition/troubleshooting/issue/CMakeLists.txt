find_package(ignition-rendering6 REQUIRED)
find_package(ignition-common4 COMPONENTS profiler events av REQUIRED)
find_package(
  Qt5
  COMPONENTS Core
             Widgets
             Gui
             OpenGL
  REQUIRED)
find_package(OpenGL REQUIRED)

qt5_wrap_cpp(MINIMAL_EXAMPLE_WIDGET_headers_MOC minimal_example.h)

add_executable(${PROJECT_NAME}_minimal_example ${MINIMAL_EXAMPLE_WIDGET_headers_MOC} main.cpp)
target_link_libraries(
  ${PROJECT_NAME}_minimal_example
  PUBLIC ignition-rendering6::ignition-rendering6
         ignition-common4::ignition-common4
         Qt5::Core
         Qt5::Widgets
         Qt5::Gui
         Qt5::OpenGL
         ${OPENGL_LIBRARIES})
target_include_directories(
  ${PROJECT_NAME}_minimal_example PUBLIC ${OPENGL_INCLUDE_DIR} "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                         "$<INSTALL_INTERFACE:include>")

install_targets(TARGETS ${PROJECT_NAME}_minimal_example)
