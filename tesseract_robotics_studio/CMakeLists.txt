cmake_minimum_required(VERSION 3.6.0)

# Extract package name and version
find_package(ros_industrial_cmake_boilerplate REQUIRED)
extract_package_metadata(pkg)
project(${pkg_extracted_name} VERSION ${pkg_extracted_version} LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(tesseract_ignition REQUIRED)
find_package(tesseract_qt REQUIRED)
find_package(tesseract_common REQUIRED)
find_package(tesseract_support REQUIRED)
find_package(tesseract_urdf REQUIRED)
find_package(qtadvanceddocking REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

qt5_wrap_cpp(TESSERACT_ROBOTICS_STUDIO_headers_MOC include/tesseract_robotics_studio/tesseract_robotics_studio.h)
qt5_wrap_ui(TESSERACT_ROBOTICS_STUDIO_headers_UI include/tesseract_robotics_studio/tesseract_robotics_studio.ui)

add_executable(
  ${PROJECT_NAME}
  ${TESSERACT_ROBOTICS_STUDIO_headers_MOC}
  ${TESSERACT_ROBOTICS_STUDIO_headers_UI}
  src/main.cpp
  src/tesseract_robotics_studio.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE tesseract::tesseract_qt_common
          tesseract::tesseract_ignition_environment_widget # for example
          tesseract::tesseract_ignition_simple_render_widget # for example
          tesseract::tesseract_support # for example
          tesseract::tesseract_urdf # for example
          tesseract::tesseract_common
          ads::qtadvanceddocking
          Qt5::Core
          Qt5::Widgets
          Qt5::Gui)
target_include_directories(
  ${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                          "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>" "$<INSTALL_INTERFACE:include>")

add_subdirectory(demo_app)

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

configure_package(NAMESPACE tesseract TARGETS ${PROJECT_NAME})
