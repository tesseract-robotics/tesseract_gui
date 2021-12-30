
find_package(tesseract_common REQUIRED)
find_package(tesseract_kinematics REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(KINEMATIC_GROUP_headers_MOC include/tesseract_gui/kinematic_group/kinematic_group_model.h)

add_library(${PROJECT_NAME}_kinematic_group SHARED
  ${KINEMATIC_GROUP_headers_MOC}
  src/opw_params_standard_item.cpp
  src/tcp_standard_item.cpp
  src/kinematic_group_model.cpp)
target_link_libraries(${PROJECT_NAME}_kinematic_group PUBLIC
  tesseract::tesseract_common
  tesseract::tesseract_kinematics_opw
  Qt5::Core Qt5::Gui
  ${PROJECT_NAME}_common
  ${PROJECT_NAME}_scene_graph)
target_include_directories(${PROJECT_NAME}_kinematic_group PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")
add_dependencies(${PROJECT_NAME}_kinematic_group ${PROJECT_NAME}_common ${PROJECT_NAME}_scene_graph)

QT5_WRAP_CPP(KINEMATIC_GROUP_EDITOR_headers_MOC
  include/tesseract_gui/kinematic_group/kinematic_group_editor_widget.h)
QT5_WRAP_UI(KINEMATIC_GROUP_EDITOR_headers_UI
  include/tesseract_gui/kinematic_group/kinematic_group_editor_widget.ui)

add_library(${PROJECT_NAME}_kinematic_group_editor_widget SHARED
  ${KINEMATIC_GROUP_EDITOR_headers_MOC}
  ${KINEMATIC_GROUP_EDITOR_headers_UI}
  src/kinematic_group_editor_widget.cpp)
target_link_libraries(${PROJECT_NAME}_kinematic_group_editor_widget PUBLIC
  Qt5::Core Qt5::Gui
  ${PROJECT_NAME}_common
  ${PROJECT_NAME}_kinematic_group)
target_include_directories(${PROJECT_NAME}_kinematic_group_editor_widget PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/kinematic_group>"
    "$<INSTALL_INTERFACE:include>")
add_dependencies(${PROJECT_NAME}_kinematic_group_editor_widget ${PROJECT_NAME}_common)

add_executable(${PROJECT_NAME}_kinematic_group_demo demo/kinematic_group_demo.cpp )
target_link_libraries(${PROJECT_NAME}_kinematic_group_demo PRIVATE
  ${PROJECT_NAME}_kinematic_group_editor_widget)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_kinematic_group ${PROJECT_NAME}_kinematic_group_editor_widget ${PROJECT_NAME}_kinematic_group_demo)
