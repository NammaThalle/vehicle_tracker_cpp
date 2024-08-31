cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(vehicle_tracking)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Add source files
set(SOURCE_FILES 
    src/logger.cpp
    src/utils.cpp
    src/video_processing.cpp
    src/object_detection.cpp
    src/vehicle_tracking.cpp
    src/main.cpp
)

# Include the headers directory
include_directories(src/headers)

# Add executable
add_executable(vehicle_tracking ${SOURCE_FILES})

# Link OpenCV libraries
target_link_libraries(vehicle_tracking ${OpenCV_LIBS})
