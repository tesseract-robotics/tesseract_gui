find_package(tesseract_common REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qwt REQUIRED)
find_package(qtadvanceddocking REQUIRED)

find_package(ignition-msgs8 REQUIRED)
find_package(tesseract_support REQUIRED)
find_package(tesseract_urdf REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_WRAP_CPP(TESSERACT_ROBOTICS_STUDIO_headers_MOC
  include/tesseract_gui/app/tesseract_robotics_studio.h)
QT5_WRAP_UI(TESSERACT_ROBOTICS_STUDIO_headers_UI
  include/tesseract_gui/app/tesseract_robotics_studio.ui)

add_executable(tesseract_robotics_studio
  ${TESSERACT_ROBOTICS_STUDIO_headers_MOC}
  ${TESSERACT_ROBOTICS_STUDIO_headers_UI}
  src/main.cpp
  src/tesseract_robotics_studio.cpp )
target_link_libraries(tesseract_robotics_studio PRIVATE
  tesseract::tesseract_common
  ads::qtadvanceddocking
  ${PROJECT_NAME}_common
  ${PROJECT_NAME}_simple_render_widget # for example
  ${PROJECT_NAME}_interactive_view_control # for example
  tesseract::tesseract_support # for example
  tesseract::tesseract_urdf # for example
  ${PROJECT_NAME}_environment # for example
  ignition-msgs8::ignition-msgs8
  Qt5::Core Qt5::Widgets Qt5::Gui)
target_include_directories(tesseract_robotics_studio PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/app>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

install_targets(TARGETS tesseract_robotics_studio)
