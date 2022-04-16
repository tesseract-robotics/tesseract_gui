find_package(tesseract_common REQUIRED)
find_package(tesseract_environment REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets Svg REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_ADD_RESOURCES(RESOURCES_common_RCC
  include/tesseract_widgets/common/resources/tesseract_widgets_resources.qrc
  include/tesseract_widgets/common/resources/themes/dark/qdarkstyle_dark.qrc
  include/tesseract_widgets/common/resources/themes/light/qdarkstyle_light.qrc)


add_library(${PROJECT_NAME}_common SHARED
  ${RESOURCES_common_RCC}
  src/calibration_info_standard_item.cpp
  src/collision_margin_data_standard_item.cpp
  src/contact_managers_plugin_info_standard_item.cpp
  src/kinematics_plugin_info_standard_item.cpp
  src/namespace_standard_item.cpp
  src/transform_standard_item.cpp
  src/pairs_collision_margin_data_standard_item.cpp
  src/plugin_info_standard_item.cpp
  src/rgba_standard_item.cpp
  src/standard_item_utils.cpp
  src/plugin_info_container_standard_item.cpp
  src/entity_manager.cpp
  src/entity_container.cpp
  src/joint_trajectory_set.cpp
  src/svg_util.cpp
)
target_link_libraries(${PROJECT_NAME}_common PUBLIC
  tesseract::tesseract_common
  tesseract::tesseract_environment
  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
target_include_directories(${PROJECT_NAME}_common PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/common>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

# Install
install_targets(TARGETS ${PROJECT_NAME}_common)
