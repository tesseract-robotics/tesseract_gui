find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL REQUIRED)
#find_package(OpenGL REQUIRED)

QT5_WRAP_CPP(MINIMAL_THREADED_WIDGET_headers_MOC renderer.h)

add_executable(${PROJECT_NAME}_minimal_threaded
  ${MINIMAL_THREADED_WIDGET_headers_MOC}
  renderer.cpp
  main.cpp)
target_link_libraries(${PROJECT_NAME}_minimal_threaded PUBLIC
  Qt5::Core Qt5::Widgets Qt5::Gui Qt5::OpenGL
   ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME}_minimal_threaded PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

install_targets(TARGETS
  ${PROJECT_NAME}_minimal_threaded)
