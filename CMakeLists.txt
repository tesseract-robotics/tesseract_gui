cmake_minimum_required(VERSION 3.6.0)

# Extract package name and version
find_package(ros_industrial_cmake_boilerplate REQUIRED)
extract_package_metadata(pkg)
project(${pkg_extracted_name} VERSION ${pkg_extracted_version} LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_subdirectory(joint_slider)
#add_subdirectory(scene_graph)
add_subdirectory(acm)
add_subdirectory(trajectory)

install(FILES "${CMAKE_CURRENT_LIST_DIR}/cmake/FindQwt.cmake" DESTINATION lib/cmake/${PROJECT_NAME})

configure_package(NAMESPACE tesseract)
