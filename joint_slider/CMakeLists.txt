
find_package(tesseract_common REQUIRED)
find_package(tesseract_scene_graph REQUIRED)
find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(JointSlider_headers_MOC include/tesseract_gui/joint_slider/joint_list_model.h)
QT5_ADD_RESOURCES(JointSlider_resources_RCC include/tesseract_gui/joint_slider/JointSlider.qrc)

add_library(${PROJECT_NAME}_joint_slider SHARED
  ${JointSlider_headers_MOC}
  ${JointSlider_resources_RCC}
  src/joint_list_model.cpp)

target_link_libraries(${PROJECT_NAME}_joint_slider PUBLIC
  tesseract::tesseract_scene_graph
  tesseract::tesseract_common
  Qt5::Core Qt5::Quick Qt5::QuickControls2 Qt5::Gui)
target_include_directories(${PROJECT_NAME}_joint_slider PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")


find_package(tesseract_support REQUIRED)
find_package(tesseract_urdf REQUIRED)

add_executable(${PROJECT_NAME}_joint_slider_demo demo/joint_slider_demo.cpp)
target_link_libraries(${PROJECT_NAME}_joint_slider_demo PRIVATE
  ${PROJECT_NAME}_joint_slider
  tesseract::tesseract_support
  tesseract::tesseract_urdf)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_joint_slider ${PROJECT_NAME}_joint_slider_demo)
