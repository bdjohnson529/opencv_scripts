cmake_minimum_required(VERSION 2.6)
project(CameraCalibration)

set(CMAKE_CXX_STANDARD 11)

# OpenCV
find_package(OpenCV 3.2.0 REQUIRED)

# output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# calibration files
include_directories(calibration)
include_directories(calibration/include)
include_directories(calibration/source)

# create calibration library
file(GLOB CALIBRATION_HEADERS "calibration/include/*.hpp")
file(GLOB CALIBRATION_SOURCES "calibration/source/*.cpp")
add_library(calibration ${CALIBRATION_SOURCES} ${CALIBRATION_HEADERS} )

# build calibration
message(STATUS ">>> Building camera calibration")
add_executable(CameraCalibration ${PROJECT_SOURCE_DIR}/calibration/tools/camera_calibration.cpp)
target_link_libraries(CameraCalibration calibration ${OpenCV_LIBS})