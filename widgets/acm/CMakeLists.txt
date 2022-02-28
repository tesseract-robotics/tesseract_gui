
find_package(tesseract_common REQUIRED)
find_package(tesseract_scene_graph REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(ACM_headers_MOC
  include/tesseract_gui/acm/allowed_collision_matrix_model.h
  include/tesseract_gui/acm/allowed_collision_matrix_widget.h
  include/tesseract_gui/acm/add_allowed_collision_entry_dialog.h)
QT5_WRAP_UI(ACM_WIDGET_headers_UI
  include/tesseract_gui/acm/allowed_collision_matrix_widget.ui
  include/tesseract_gui/acm/add_allowed_collision_entry_dialog.ui)

add_library(${PROJECT_NAME}_acm SHARED
  ${ACM_headers_MOC}
  ${ACM_WIDGET_headers_UI}
  src/allowed_collision_matrix_model.cpp
  src/allowed_collision_matrix_widget.cpp
  src/add_allowed_collision_entry_dialog.cpp)

target_link_libraries(${PROJECT_NAME}_acm PUBLIC
  tesseract::tesseract_scene_graph
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets Qt5::Gui)
target_include_directories(${PROJECT_NAME}_acm PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/widgets/acm>"
    "$<INSTALL_INTERFACE:include>")

add_subdirectory(demo)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

# Install
install_targets(TARGETS ${PROJECT_NAME}_acm)
