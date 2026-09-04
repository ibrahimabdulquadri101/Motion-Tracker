cmake_minimum_required(VERSION 3.16)
project(motion-detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(
    src
    ${OpenCV_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/FrameCapture.cpp
    src/BackgroundModel.cpp
    src/BlobDetector.cpp
    src/KalmanTracker.cpp
    src/Detector.cpp
    src/Alerter.cpp
)

# Header files
set(HEADERS
    src/FrameCapture.h
    src/BackgroundModel.h
    src/BlobDetector.h
    src/KalmanTracker.h
    src/BoundingBox.h
    src/Detector.h
    src/Alerter.h
)

# Executable target
add_executable(motion-detector ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(motion-detector PRIVATE ${OpenCV_LIBS})
