cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project (JigsawVisual)


set(OpenCV_DIR D:/opencv3.4.1/opencv/build)
find_package(OpenCV REQUIRED)

set(EIGEN3_INCLUDE_DIR D:/3rdParty/Eigen3.3.4)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(VisLib JigsawVisual.cpp JigsawVisualPair.cpp)


add_executable(main_AssembleLoop main_AssembleLoop.cpp)
add_executable(main_AssemblePair main_AssemblePair.cpp)
add_executable(main_AssemblePose main_AssemblePose.cpp)
add_executable(main_AssembleTopKPair main_AssembleTopKPair.cpp)

target_link_libraries(main_AssembleLoop VisLib ${OpenCV_LIBS})
target_link_libraries(main_AssemblePair VisLib ${OpenCV_LIBS})
target_link_libraries(main_AssemblePose VisLib ${OpenCV_LIBS})
target_link_libraries(main_AssembleTopKPair VisLib ${OpenCV_LIBS})

