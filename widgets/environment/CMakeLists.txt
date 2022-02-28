find_package(tesseract_common REQUIRED)
find_package(tesseract_environment REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(ENVIRONMENT_WIDGET_headers_MOC include/tesseract_gui/environment/environment_widget.h)
QT5_WRAP_UI(ENVIRONMENT_WIDGET_headers_UI include/tesseract_gui/environment/environment_widget.ui)

add_library(${PROJECT_NAME}_environment SHARED
  ${ENVIRONMENT_WIDGET_headers_MOC}
  ${ENVIRONMENT_WIDGET_headers_UI}
  src/environment_widget.cpp)
target_link_libraries(${PROJECT_NAME}_environment PUBLIC
  tesseract::tesseract_environment
  tesseract::tesseract_common
  Qt5::Core Qt5::Gui Qt5::Widgets
  ${PROJECT_NAME}_common
  ${PROJECT_NAME}_scene_graph
  ${PROJECT_NAME}_kinematic_groups
  ${PROJECT_NAME}_acm)
target_include_directories(${PROJECT_NAME}_environment PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/widgets/environment>"
    "$<INSTALL_INTERFACE:include>")

add_subdirectory(demo)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

# Install
install_targets(TARGETS ${PROJECT_NAME}_environment)
