cmake_minimum_required(VERSION 3.6.0)

# Extract package name and version
find_package(ros_industrial_cmake_boilerplate REQUIRED)
extract_package_metadata(pkg)
project(${pkg_extracted_name} VERSION ${pkg_extracted_version} LANGUAGES CXX)

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

find_package(tesseract_common REQUIRED)
find_package(tesseract_scene_graph REQUIRED)
find_package(tesseract_widgets REQUIRED)

find_package(ignition-gui6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)
find_package(ignition-common4 COMPONENTS profiler events av REQUIRED)
find_package(ignition-transport11 REQUIRED)
find_package(ignition-msgs8 REQUIRED)
find_package(ignition-math6-eigen3 REQUIRED)
find_package(sdformat12 REQUIRED)

find_package(
  Qt5
  COMPONENTS Core
             Widgets
             QuickWidgets
             Quick
             QuickControls2
  REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

add_library(${PROJECT_NAME}_common SHARED src/conversions.cpp src/gui_events.cpp src/gui_utils.cpp)
target_link_libraries(
  ${PROJECT_NAME}_common
  PUBLIC tesseract::tesseract_widgets_common
         tesseract::tesseract_common
         tesseract::tesseract_scene_graph
         ignition-common4::ignition-common4-profiler
         ignition-common4::ignition-common4-events
         ignition-common4::ignition-common4-av
         ignition-gui6::ignition-gui6
         ignition-rendering6::ignition-rendering6
         ignition-transport11::ignition-transport11
         ignition-msgs8::ignition-msgs8
         ignition-math6::ignition-math6-eigen3
         Qt5::Core
         Qt5::Widgets)
target_include_directories(${PROJECT_NAME}_common PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:include>")

qt5_wrap_cpp(INTERACTIVE_VIEW_CONTROL_headers_MOC include/tesseract_ignition/interactive_view_control.h)

add_library(${PROJECT_NAME}_interactive_view_control SHARED ${INTERACTIVE_VIEW_CONTROL_headers_MOC}
                                                            src/interactive_view_control.cpp)
target_link_libraries(
  ${PROJECT_NAME}_interactive_view_control
  PUBLIC ${PROJECT_NAME}_common
         ignition-common4::ignition-common4-profiler
         ignition-common4::ignition-common4-events
         ignition-common4::ignition-common4-av
         ignition-gui6::ignition-gui6
         ignition-rendering6::ignition-rendering6
         ignition-transport11::ignition-transport11
         ignition-msgs8::ignition-msgs8
         ignition-math6::ignition-math6-eigen3
         Qt5::Core
         Qt5::Widgets)
target_include_directories(
  ${PROJECT_NAME}_interactive_view_control PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                  "$<INSTALL_INTERFACE:include>")

qt5_wrap_cpp(MINIMAL_SCENE_headers_MOC include/tesseract_ignition/minimal_scene.h)
qt5_add_resources(MINIMAL_SCENE_resources_RCC include/tesseract_ignition/minimal_scene.qrc)

add_library(${PROJECT_NAME}_qquick_render_widget SHARED ${MINIMAL_SCENE_headers_MOC} ${MINIMAL_SCENE_resources_RCC}
                                                        src/minimal_scene.cpp)
target_link_libraries(
  ${PROJECT_NAME}_qquick_render_widget
  PUBLIC ${PROJECT_NAME}_common
         ${PROJECT_NAME}_interactive_view_control
         ignition-common4::ignition-common4-profiler
         ignition-common4::ignition-common4-events
         ignition-common4::ignition-common4-av
         ignition-gui6::ignition-gui6
         ignition-rendering6::ignition-rendering6
         ignition-transport11::ignition-transport11
         ignition-msgs8::ignition-msgs8
         ignition-math6::ignition-math6-eigen3
         sdformat12::sdformat12
         Qt5::Core
         Qt5::Quick
         Qt5::QuickControls2)
target_include_directories(
  ${PROJECT_NAME}_qquick_render_widget PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                              "$<INSTALL_INTERFACE:include>")

qt5_wrap_cpp(THREADED_RENDER_WIDGET_headers_MOC include/tesseract_ignition/render_widget.h src/render_widget.cpp)
add_library(${PROJECT_NAME}_threaded_render_widget SHARED ${THREADED_RENDER_WIDGET_headers_MOC} src/render_widget.cpp)
target_link_libraries(
  ${PROJECT_NAME}_threaded_render_widget
  PUBLIC ${PROJECT_NAME}_common
         ${PROJECT_NAME}_interactive_view_control
         ignition-common4::ignition-common4-profiler
         ignition-common4::ignition-common4-events
         ignition-common4::ignition-common4-av
         ignition-gui6::ignition-gui6
         ignition-rendering6::ignition-rendering6
         ignition-transport11::ignition-transport11
         ignition-msgs8::ignition-msgs8
         ignition-math6::ignition-math6-eigen3
         sdformat12::sdformat12
         Qt5::Core
         Qt5::Widgets)
target_include_directories(
  ${PROJECT_NAME}_threaded_render_widget PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                "$<INSTALL_INTERFACE:include>")

qt5_wrap_cpp(SIMPLE_RENDER_WIDGET_headers_MOC include/tesseract_ignition/simple_render_widget.h)
add_library(${PROJECT_NAME}_simple_render_widget SHARED ${SIMPLE_RENDER_WIDGET_headers_MOC}
                                                        src/simple_render_widget.cpp)
target_link_libraries(
  ${PROJECT_NAME}_simple_render_widget
  PUBLIC ${PROJECT_NAME}_common
         ${PROJECT_NAME}_interactive_view_control
         ignition-common4::ignition-common4-profiler
         ignition-common4::ignition-common4-events
         ignition-common4::ignition-common4-av
         ignition-gui6::ignition-gui6
         ignition-rendering6::ignition-rendering6
         ignition-transport11::ignition-transport11
         ignition-msgs8::ignition-msgs8
         ignition-math6::ignition-math6-eigen3
         sdformat12::sdformat12
         Qt5::Core
         Qt5::Widgets)
target_include_directories(
  ${PROJECT_NAME}_simple_render_widget PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                              "$<INSTALL_INTERFACE:include>")

find_package(GLUT REQUIRED)
include_directories(SYSTEM ${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})

if(NOT APPLE)
  find_package(GLEW REQUIRED)
  include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})
  link_directories(${GLEW_LIBRARY_DIRS})
endif()

qt5_wrap_cpp(GLUT_RENDER_WIDGET_headers_MOC include/tesseract_ignition/glut_render_widget.h)
add_library(${PROJECT_NAME}_glut_render_widget SHARED ${GLUT_RENDER_WIDGET_headers_MOC} src/glut_render_widget.cpp)
target_link_libraries(
  ${PROJECT_NAME}_glut_render_widget
  PUBLIC ${PROJECT_NAME}_common
         ${PROJECT_NAME}_interactive_view_control
         ${GLUT_LIBRARIES}
         ${OPENGL_LIBRARIES}
         ${GLEW_LIBRARIES}
         ignition-common4::ignition-common4-profiler
         ignition-common4::ignition-common4-events
         ignition-common4::ignition-common4-av
         ignition-gui6::ignition-gui6
         ignition-rendering6::ignition-rendering6
         ignition-transport11::ignition-transport11
         ignition-msgs8::ignition-msgs8
         ignition-math6::ignition-math6-eigen3
         sdformat12::sdformat12
         Qt5::Core
         Qt5::Widgets)
target_include_directories(
  ${PROJECT_NAME}_glut_render_widget PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                            "$<INSTALL_INTERFACE:include>")
target_include_directories(${PROJECT_NAME}_glut_render_widget SYSTEM PUBLIC ${GLUT_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS}
                                                                            ${GLEW_LIBRARY_DIRS})

qt5_wrap_cpp(IGNITION_ENVIRONMENT_WIDGET_headers_MOC include/tesseract_ignition/ignition_environment_widget.h)

add_library(${PROJECT_NAME}_environment_widget SHARED ${IGNITION_ENVIRONMENT_WIDGET_headers_MOC}
                                                      src/ignition_environment_widget.cpp)
target_link_libraries(
  ${PROJECT_NAME}_environment_widget
  PUBLIC ${PROJECT_NAME}_common
         tesseract::tesseract_widgets_environment
         tesseract::tesseract_widgets_common
         ignition-rendering6::ignition-rendering6
         ignition-math6::ignition-math6-eigen3
         ignition-common4::ignition-common4)

target_include_directories(
  ${PROJECT_NAME}_environment_widget PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                            "$<INSTALL_INTERFACE:include>")

add_subdirectory(demo)
add_subdirectory(troubleshooting/issue)
add_subdirectory(troubleshooting/threaded)
add_subdirectory(troubleshooting/glut)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

install_targets(
  TARGETS
  ${PROJECT_NAME}_common
  ${PROJECT_NAME}_interactive_view_control
  ${PROJECT_NAME}_qquick_render_widget
  ${PROJECT_NAME}_threaded_render_widget
  ${PROJECT_NAME}_simple_render_widget
  ${PROJECT_NAME}_glut_render_widget
  ${PROJECT_NAME}_environment_widget)

configure_package(
  NAMESPACE tesseract
            DEPENDENCIES
            tesseract_common
            tesseract_scene_graph
            tesseract_widgets
            ignition-gui6
            ignition-rendering6
            "ignition-common4 COMPONENTS profiler events av"
            ignition-transport11
            ignition-msgs8
            ignition-math6-eigen3
            sdformat12)
