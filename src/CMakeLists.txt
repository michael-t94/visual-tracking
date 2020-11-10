cmake_minimum_required(VERSION 2.8)

# Path to the location where OpenCVConfig.cmake resides in
set(OpenCV_DIR /Users/michael/opencv4.5.0/release_d)
# Generate compilation database to facilitate YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

project(Tracking)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(DTracking detecting_tracking.cpp)
target_link_libraries(DTracking ${OpenCV_LIBS})
