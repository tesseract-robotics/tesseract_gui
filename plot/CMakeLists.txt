find_package(tesseract_common REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Xml REQUIRED)
find_package(Qwt REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_UI(PLOT_headers_UI
  include/tesseract_gui/plot/plot_widget_editor.ui
  include/tesseract_gui/plot/plot_widget_transforms.ui
  )

QT5_WRAP_CPP(PLOT_headers_MOC
  include/tesseract_gui/plot/custom_tracker.h
  include/tesseract_gui/plot/plot_legend.h
  include/tesseract_gui/plot/plot_magnifier.h
  include/tesseract_gui/plot/plot_widget.h
  include/tesseract_gui/plot/plot_widget_base.h
  include/tesseract_gui/plot/plot_widget_editor.h
  include/tesseract_gui/plot/plot_widget_transforms.h
  include/tesseract_gui/plot/plot_zoomer.h
  include/tesseract_gui/plot/transform_function.h
  )

add_library(${PROJECT_NAME}_plot SHARED
  ${PLOT_headers_UI}
  ${PLOT_headers_MOC}
  src/custom_tracker.cpp
  src/plot_data.cpp
  src/plot_legend.cpp
  src/plot_magnifier.cpp
  src/plot_widget.cpp
  src/plot_widget_base.cpp
  src/plot_widget_editor.cpp
  src/plot_widget_transforms.cpp
  src/plot_zoomer.cpp
  src/point_series_xy.cpp
  src/timeseries_qwt.cpp
  src/transform_function.cpp)
target_cxx_version(${PROJECT_NAME}_plot PUBLIC VERSION ${TESSERACT_CXX_VERSION})
target_link_libraries(${PROJECT_NAME}_plot PUBLIC
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::OpenGL
  Qt5::Xml
  Qwt::Qwt
  tesseract::tesseract_common
  ${PROJECT_NAME}_resources)
target_include_directories(${PROJECT_NAME}_plot PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/plot>"
    "$<INSTALL_INTERFACE:include>")
add_dependencies(${PROJECT_NAME}_plot ${PROJECT_NAME}_resources)

add_executable(${PROJECT_NAME}_plot_demo demo/plot_demo.cpp )
target_link_libraries(${PROJECT_NAME}_plot_demo PRIVATE
  ${PROJECT_NAME}_plot)
target_include_directories(${PROJECT_NAME}_plot_demo PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_plot ${PROJECT_NAME}_plot_demo)
