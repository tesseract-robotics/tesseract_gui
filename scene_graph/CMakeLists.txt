
find_package(tesseract_common REQUIRED)
find_package(tesseract_scene_graph REQUIRED)
find_package(tesseract_support REQUIRED) # demo
find_package(tesseract_urdf REQUIRED)    # demo
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

add_library(${PROJECT_NAME}_scene_graph_model SHARED
  src/calibration_standard_item.cpp
  src/collision_standard_item.cpp
  src/dynamics_standard_item.cpp
  src/inertial_standard_item.cpp
  src/joint_standard_item.cpp
  src/limits_standard_item.cpp
  src/link_standard_item.cpp
  src/material_standard_item.cpp
  src/mimic_standard_item.cpp
  src/origin_standard_item.cpp
  src/safety_standard_item.cpp
  src/scene_graph_standard_item.cpp
  src/visual_standard_item.cpp
  src/box_standard_item.cpp
  src/capsule_standard_item.cpp
  src/cone_standard_item.cpp
  src/cylinder_standard_item.cpp
  src/plane_standard_item.cpp
  src/sphere_standard_item.cpp
  src/polygon_mesh_standard_item.cpp
  src/octree_standard_item.cpp
)

target_link_libraries(${PROJECT_NAME}_scene_graph_model PUBLIC
  tesseract::tesseract_scene_graph
  tesseract::tesseract_common
  Qt5::Core Qt5::Gui Qt5::Widgets
  ${PROJECT_NAME}_resources)
target_include_directories(${PROJECT_NAME}_scene_graph_model PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
add_dependencies(${PROJECT_NAME}_scene_graph_model ${PROJECT_NAME}_resources)

add_executable(${PROJECT_NAME}_scene_graph_demo demo/scene_graph_demo.cpp )
target_link_libraries(${PROJECT_NAME}_scene_graph_demo PRIVATE
  tesseract::tesseract_support
  tesseract::tesseract_urdf
  ${PROJECT_NAME}_scene_graph_model
  ${PROJECT_NAME}_resources)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_scene_graph_model ${PROJECT_NAME}_scene_graph_demo)
