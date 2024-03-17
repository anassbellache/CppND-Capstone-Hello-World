cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++20)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include directories for Eigen and LibTorch
include_directories(${CMAKE_SOURCE_DIR}/libs/eigen)
include_directories(${CMAKE_SOURCE_DIR}/libs/libtorch/include)

project(ImageDenoiser LANGUAGES C CXX)

add_subdirectory("src/plugins")
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(OpenCV 4.1 REQUIRED)


add_executable(ImageDenoiser
        src/mainwindow.cpp
        src/mainwindow.h
        src/main.cpp)

# Link LibTorch library
target_link_libraries(ImageDenoiser PRIVATE ${CMAKE_SOURCE_DIR}/libs/libtorch/lib/libtorch.so Qt6::Core Qt6::Gui Qt6::Widgets)