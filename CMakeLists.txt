cmake_minimum_required(VERSION 3.28)
project(Hit-cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

#add_definitions(-DDEBUG)
#add_definitions(-DSHOW)

set(OpenCV_DIR /opt/homebrew/Cellar/opencv/4.10.0)
find_package(OpenCV REQUIRED)

find_package(Eigen3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(Include)

add_executable(Hit-cpp main.cpp
        Include/imageHandler.h
        Src/imageHandler.cpp
        Include/fileHandler.h
        Src/fileHandler.cpp
        Include/minCircleSolver.h
        Src/minCircleSolver.cpp
        Include/lineSolver.h
        Src/lineSolver.cpp)

target_link_libraries(Hit-cpp ${OpenCV_LIBS} Eigen3::Eigen)
