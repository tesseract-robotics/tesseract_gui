find_package(tesseract_common REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

add_library(${PROJECT_NAME}_common_widgets SHARED
  src/calibration_info_standard_item.cpp
  src/collision_margin_data_standard_item.cpp
  src/contact_managers_plugin_info_standard_item.cpp
  src/kinematics_plugin_info_standard_item.cpp
  src/transform_standard_item.cpp
  src/pairs_collision_margin_data_standard_item.cpp
  src/plugin_info_standard_item.cpp
  src/rgba_standard_item.cpp
  src/standard_item_utils.cpp
  src/plugin_info_container_standard_item.cpp
)
target_link_libraries(${PROJECT_NAME}_common_widgets PUBLIC
  tesseract::tesseract_common
  Qt5::Core Qt5::Gui Qt5::Widgets
  ${PROJECT_NAME}_common)
target_include_directories(${PROJECT_NAME}_common_widgets PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/widgets/common>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

# Install
install_targets(TARGETS ${PROJECT_NAME}_common_widgets)
