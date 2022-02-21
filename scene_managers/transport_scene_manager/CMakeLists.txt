
find_package(tesseract_common REQUIRED)

find_package(ignition-gui6 REQUIRED)
find_package(ignition-rendering6 REQUIRED)
find_package(ignition-common4 COMPONENTS profiler events av REQUIRED)
find_package(ignition-transport11 REQUIRED)
find_package(ignition-msgs8 REQUIRED)
find_package(ignition-math6-eigen3 REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(TRANSPORT_SCENE_MANAGER_headers_MOC
  include/tesseract_gui/transport_scene_manager/transport_scene_manager.h
  include/tesseract_gui/transport_scene_manager/transport_marker_manager.h)

add_library(${PROJECT_NAME}_transport_scene_manager SHARED
  ${TRANSPORT_SCENE_MANAGER_headers_MOC}
  src/transport_conversions.cpp
  src/transport_scene_manager.cpp
  src/transport_marker_manager.cpp)
target_link_libraries(${PROJECT_NAME}_transport_scene_manager PUBLIC
  ${PROJECT_NAME}_common
  tesseract::tesseract_common
  ignition-common4::ignition-common4-profiler
  ignition-common4::ignition-common4-events
  ignition-common4::ignition-common4-av
  ignition-gui6::ignition-gui6
  ignition-rendering6::ignition-rendering6
  ignition-transport11::ignition-transport11
  ignition-msgs8::ignition-msgs8
  ignition-math6::ignition-math6-eigen3)
target_include_directories(${PROJECT_NAME}_transport_scene_manager PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")


find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(tesseract_urdf REQUIRED)
find_package(tesseract_support REQUIRED)
add_executable(${PROJECT_NAME}_transport_scene_manager_demo demo/transport_scene_manager_demo.cpp)
target_link_libraries(${PROJECT_NAME}_transport_scene_manager_demo PRIVATE
  ${PROJECT_NAME}_transport_scene_manager
  ${PROJECT_NAME}_simple_render_widget
  tesseract::tesseract_common
  tesseract::tesseract_urdf
  tesseract::tesseract_support
  Qt5::Core Qt5::Widgets)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_transport_scene_manager ${PROJECT_NAME}_transport_scene_manager_demo)
