add_executable(${PROJECT_NAME}_qquick_render_widget_demo qquick_render_widget_demo.cpp)
target_link_libraries(${PROJECT_NAME}_qquick_render_widget_demo PRIVATE
  ${PROJECT_NAME}_qquick_render_widget
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets Qt5::QuickWidgets)

add_executable(${PROJECT_NAME}_simple_render_widget_demo simple_render_widget_demo.cpp)
target_link_libraries(${PROJECT_NAME}_simple_render_widget_demo PRIVATE
  ${PROJECT_NAME}_simple_render_widget
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets)

add_executable(${PROJECT_NAME}_threaded_render_widget_demo threaded_render_widget_demo.cpp)
target_link_libraries(${PROJECT_NAME}_threaded_render_widget_demo PRIVATE
  ${PROJECT_NAME}_threaded_render_widget
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets)

add_executable(${PROJECT_NAME}_glut_render_widget_demo glut_render_widget_demo.cpp)
target_link_libraries(${PROJECT_NAME}_glut_render_widget_demo PRIVATE
  ${PROJECT_NAME}_glut_render_widget
  tesseract::tesseract_common
  Qt5::Core Qt5::Widgets)

install_targets(TARGETS
  ${PROJECT_NAME}_qquick_render_widget_demo
  ${PROJECT_NAME}_simple_render_widget_demo
  ${PROJECT_NAME}_glut_render_widget_demo
  ${PROJECT_NAME}_threaded_render_widget_demo)
