cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

set(OpenCV_DIR /home/lecanyu/Downloads/opencv-3.4.1/build/release/share/OpenCV)
find_package(OpenCV REQUIRED)

set(EIGEN3_INCLUDE_DIR /usr/include/eigen3)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

project (JigsawVisual)
aux_source_directory(. src)
add_executable(JigsawVisual ${src})

target_link_libraries(JigsawVisual ${OpenCV_LIBS})