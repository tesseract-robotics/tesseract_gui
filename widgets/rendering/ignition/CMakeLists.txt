
find_package(tesseract_common REQUIRED)

find_package(ignition-gui6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)
find_package(ignition-common4 COMPONENTS profiler events av REQUIRED)
find_package(ignition-transport11 REQUIRED)
find_package(ignition-msgs8 REQUIRED)
find_package(ignition-math6-eigen3 REQUIRED)
find_package(sdformat12 REQUIRED)

find_package(Qt5 COMPONENTS Core Widgets QuickWidgets Quick QuickControls2 REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(INTERACTIVE_VIEW_CONTROL_headers_MOC
  include/tesseract_gui/rendering/ignition/interactive_view_control.h)

add_library(${PROJECT_NAME}_interactive_view_control SHARED
  ${INTERACTIVE_VIEW_CONTROL_headers_MOC}
  src/interactive_view_control.cpp)
target_link_libraries(${PROJECT_NAME}_interactive_view_control PUBLIC
  ${PROJECT_NAME}_common
  ignition-common4::ignition-common4-profiler
  ignition-common4::ignition-common4-events
  ignition-common4::ignition-common4-av
  ignition-gui6::ignition-gui6
  ignition-rendering6::ignition-rendering6
  ignition-transport11::ignition-transport11
  ignition-msgs8::ignition-msgs8
  ignition-math6::ignition-math6-eigen3
  Qt5::Core Qt5::Widgets)
target_include_directories(${PROJECT_NAME}_interactive_view_control PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

QT5_WRAP_CPP(MINIMAL_SCENE_headers_MOC
  include/tesseract_gui/rendering/ignition/minimal_scene.h)
QT5_ADD_RESOURCES(MINIMAL_SCENE_resources_RCC include/tesseract_gui/rendering/ignition/minimal_scene.qrc)

add_library(${PROJECT_NAME}_qquick_render_widget SHARED
  ${MINIMAL_SCENE_headers_MOC}
  ${MINIMAL_SCENE_resources_RCC}
  src/minimal_scene.cpp)
target_link_libraries(${PROJECT_NAME}_qquick_render_widget PUBLIC
  ${PROJECT_NAME}_common
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
  Qt5::Core Qt5::Quick Qt5::QuickControls2 )
target_include_directories(${PROJECT_NAME}_qquick_render_widget PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

QT5_WRAP_CPP(THREADED_RENDER_WIDGET_headers_MOC
  include/tesseract_gui/rendering/ignition/render_widget.h
  src/render_widget.cpp)
add_library(${PROJECT_NAME}_threaded_render_widget SHARED
  ${THREADED_RENDER_WIDGET_headers_MOC}
  src/render_widget.cpp)
target_link_libraries(${PROJECT_NAME}_threaded_render_widget PUBLIC
  ${PROJECT_NAME}_common
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
  Qt5::Core Qt5::Widgets)
target_include_directories(${PROJECT_NAME}_threaded_render_widget PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

QT5_WRAP_CPP(SIMPLE_RENDER_WIDGET_headers_MOC
  include/tesseract_gui/rendering/ignition/simple_render_widget.h)
add_library(${PROJECT_NAME}_simple_render_widget SHARED
  ${SIMPLE_RENDER_WIDGET_headers_MOC}
  src/simple_render_widget.cpp)
target_link_libraries(${PROJECT_NAME}_simple_render_widget PUBLIC
  ${PROJECT_NAME}_common
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
  Qt5::Core Qt5::Widgets)
target_include_directories(${PROJECT_NAME}_simple_render_widget PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

add_subdirectory(demo)
add_subdirectory(troubleshooting/issue)
add_subdirectory(troubleshooting/threaded)
add_subdirectory(troubleshooting/glut)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

install_targets(TARGETS
  ${PROJECT_NAME}_interactive_view_control
  ${PROJECT_NAME}_qquick_render_widget
  ${PROJECT_NAME}_threaded_render_widget
  ${PROJECT_NAME}_simple_render_widget)
