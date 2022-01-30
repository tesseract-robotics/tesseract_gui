find_package(tesseract_common REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Svg REQUIRED)

# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

QT5_ADD_RESOURCES(RESOURCES_common_RCC include/tesseract_gui/common/resources/resources.qrc)

add_library(${PROJECT_NAME}_common SHARED
  ${RESOURCES_common_RCC}
  src/entity_manager.cpp
  src/gui_utils.cpp
  src/svg_util.cpp)
target_cxx_version(${PROJECT_NAME}_common PUBLIC VERSION ${TESSERACT_CXX_VERSION})
target_link_libraries(${PROJECT_NAME}_common PUBLIC
  Qt5::Core
  Qt5::Gui
  Qt5::Svg
  tesseract::tesseract_common)
target_include_directories(${PROJECT_NAME}_common PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>")

# Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION include
  PATTERN ".svn" EXCLUDE
)

install_targets(TARGETS ${PROJECT_NAME}_common)
