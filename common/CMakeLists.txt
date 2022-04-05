find_package(tesseract_common REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Svg REQUIRED)
find_package(ignition-common4 COMPONENTS events REQUIRED)
find_package(ignition-math6-eigen3 REQUIRED)
find_package(ignition-rendering6 REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_ADD_RESOURCES(RESOURCES_common_RCC
  include/tesseract_gui/common/resources/tesseract_gui_resources.qrc
  include/tesseract_gui/common/resources/themes/dark/qdarkstyle_dark.qrc
  include/tesseract_gui/common/resources/themes/light/qdarkstyle_light.qrc)

add_library(${PROJECT_NAME}_common SHARED
  ${RESOURCES_common_RCC}
  src/conversions.cpp
  src/entity_manager.cpp
  src/entity_container.cpp
  src/gui_utils.cpp
  src/gui_events.cpp
  src/joint_trajectory_set.cpp
  src/svg_util.cpp)
target_cxx_version(${PROJECT_NAME}_common PUBLIC VERSION ${TESSERACT_CXX_VERSION})
target_link_libraries(${PROJECT_NAME}_common PUBLIC
  Qt5::Core
  Qt5::Gui
  Qt5::Svg
  tesseract::tesseract_common
  ignition-common4::ignition-common4-events
  ignition-math6::ignition-math6-eigen3
  ignition-rendering6::ignition-rendering6)
target_include_directories(${PROJECT_NAME}_common PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

install_targets(TARGETS ${PROJECT_NAME}_common)
