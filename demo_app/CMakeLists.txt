find_package(tesseract_common REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL Xml REQUIRED)
find_package(Qwt REQUIRED)
find_package(qtadvanceddocking REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_UI(PLOT_headers_UI
  include/tesseract_gui/demo_app/mainwindow.ui
  include/tesseract_gui/demo_app/StatusDialog.ui
  )

QT5_WRAP_CPP(PLOT_headers_MOC
  include/tesseract_gui/demo_app/MainWindow.h
  include/tesseract_gui/demo_app/StatusDialog.h
  )

add_library(${PROJECT_NAME}_demo_app SHARED
  ${PLOT_headers_UI}
  ${PLOT_headers_MOC}
  src/MainWindow.cpp
  src/StatusDialog.cpp
  )
target_cxx_version(${PROJECT_NAME}_demo_app PUBLIC VERSION ${TESSERACT_CXX_VERSION})
target_link_libraries(${PROJECT_NAME}_demo_app PUBLIC
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::OpenGL
  Qt5::Xml
  tesseract::tesseract_common
#  qt_advanced_docking
  ads::qtadvanceddocking
  ${PROJECT_NAME}_common)
target_include_directories(${PROJECT_NAME}_demo_app PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/demo_app>"
    "$<INSTALL_INTERFACE:include>")

add_executable(${PROJECT_NAME}_demo_app_demo src/main.cpp )
target_link_libraries(${PROJECT_NAME}_demo_app_demo PRIVATE ${PROJECT_NAME}_demo_app)
target_include_directories(${PROJECT_NAME}_demo_app_demo PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.qml" PATTERN "*.qrc"
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS
  ${PROJECT_NAME}_demo_app
  ${PROJECT_NAME}_demo_app_demo)
