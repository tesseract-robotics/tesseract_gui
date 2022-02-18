
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

QT5_WRAP_CPP(MINIMAL_SCENE_headers_MOC
  include/tesseract_gui/rendering/minimal_scene.h
  include/tesseract_gui/rendering/interactive_view_control.h)
QT5_ADD_RESOURCES(MINIMAL_SCENE_resources_RCC include/tesseract_gui/rendering/minimal_scene.qrc)

add_library(${PROJECT_NAME}_rendering_widget SHARED
  ${MINIMAL_SCENE_headers_MOC}
  ${MINIMAL_SCENE_resources_RCC}
  src/minimal_scene.cpp
  src/interactive_view_control.cpp)
target_link_libraries(${PROJECT_NAME}_rendering_widget PUBLIC
  ${PROJECT_NAME}_common
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
target_include_directories(${PROJECT_NAME}_rendering_widget PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

QT5_WRAP_CPP(RENDER_WIDGET_headers_MOC include/tesseract_gui/rendering/render_widget.h)
add_library(${PROJECT_NAME}_rendering_widget2 SHARED
  ${RENDER_WIDGET_headers_MOC}
  src/render_widget.cpp)
target_link_libraries(${PROJECT_NAME}_rendering_widget2 PUBLIC
  ${PROJECT_NAME}_common
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
target_include_directories(${PROJECT_NAME}_rendering_widget2 PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

QT5_WRAP_CPP(SIMPLE_RENDER_WIDGET_headers_MOC
  include/tesseract_gui/rendering/simple_render_widget.h
  include/tesseract_gui/rendering/interactive_view_control.h)
add_library(${PROJECT_NAME}_rendering_widget3 SHARED
  ${SIMPLE_RENDER_WIDGET_headers_MOC}
  src/simple_render_widget.cpp
  src/interactive_view_control.cpp)
target_link_libraries(${PROJECT_NAME}_rendering_widget3 PUBLIC
  ${PROJECT_NAME}_common
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
target_include_directories(${PROJECT_NAME}_rendering_widget3 PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

add_executable(${PROJECT_NAME}_rendering_demo demo/rendering_demo.cpp)
target_link_libraries(${PROJECT_NAME}_rendering_demo PRIVATE
  ${PROJECT_NAME}_rendering_widget
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets Qt5::QuickWidgets)

add_executable(${PROJECT_NAME}_render_widget_demo demo/render_widget_demo.cpp)
target_link_libraries(${PROJECT_NAME}_render_widget_demo PRIVATE
  ${PROJECT_NAME}_rendering_widget3
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS
  ${PROJECT_NAME}_rendering_widget
  ${PROJECT_NAME}_rendering_widget2
  ${PROJECT_NAME}_rendering_widget3
  ${PROJECT_NAME}_rendering_demo
  ${PROJECT_NAME}_render_widget_demo)
