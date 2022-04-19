find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(tesseract_urdf REQUIRED)
find_package(tesseract_support REQUIRED)

add_executable(${PROJECT_NAME}_transport_scene_manager_demo transport_scene_manager_demo.cpp)
target_link_libraries(
  ${PROJECT_NAME}_transport_scene_manager_demo
  PRIVATE ${PROJECT_NAME}_transport_scene_manager
          ${PROJECT_NAME}_simple_render_widget
          tesseract::tesseract_common
          tesseract::tesseract_urdf
          tesseract::tesseract_support
          Qt5::Core
          Qt5::Widgets)

install_targets(TARGETS ${PROJECT_NAME}_transport_scene_manager_demo)
