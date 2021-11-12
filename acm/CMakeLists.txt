
find_package(tesseract_common REQUIRED)
find_package(tesseract_scene_graph REQUIRED)
find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(ACM_headers_MOC include/tesseract_gui/acm/allowed_collision_matrix_model.h)
QT5_ADD_RESOURCES(ACM_resources_RCC include/tesseract_gui/acm/AllowedCollisionMatrix.qrc)

add_library(${PROJECT_NAME}_acm SHARED
  ${ACM_headers_MOC}
  ${ACM_resources_RCC}
  src/allowed_collision_matrix_model.cpp)

target_link_libraries(${PROJECT_NAME}_acm PUBLIC
  tesseract::tesseract_scene_graph
  tesseract::tesseract_common
  Qt5::Core Qt5::Quick Qt5::QuickControls2 Qt5::Gui)
target_include_directories(${PROJECT_NAME}_acm PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

add_executable(${PROJECT_NAME}_acm_demo demo/acm_demo.cpp )
target_link_libraries(${PROJECT_NAME}_acm_demo PRIVATE
  ${PROJECT_NAME}_acm)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_acm ${PROJECT_NAME}_acm_demo)
