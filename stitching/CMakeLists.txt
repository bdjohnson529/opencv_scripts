project(platform)
cmake_minimum_required(VERSION 2.6)

# OpenCV
find_package(OpenCV 3.2.0 REQUIRED)
find_package(Qt5Widgets)

# OpenCV
find_package(Eigen3 REQUIRED)

# Boost
find_package(Boost 1.54 COMPONENTS iostreams program_options filesystem)

# set properties
include_directories(include
)

add_definitions(-std=c++11 -O3)
link_directories(
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/libproto/lib
	${PROJECT_SOURCE_DIR}/coordinates/lib
	/opt/pleora/ebus_sdk/Ubuntu-x86_64/lib
)

# output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# grab files
file(GLOB PLATFORM_HEADERS "include/*.hpp")
file(GLOB PLATFORM_SOURCES "source/*.cpp")

# create stitching lib
add_library(stitching STATIC ${PLATFORM_SOURCES} ${PLATFORM_HEADERS})
target_link_libraries(stitching ${OpenCV_LIBS})

# stitching
add_executable(oksi_stitcher ${PROJECT_SOURCE_DIR}/source/oksi_stitcher.cpp)
target_link_libraries(oksi_stitcher ${OpenCV_LIBS})
add_executable(opencv_stitcher ${PROJECT_SOURCE_DIR}/source/opencv_stitcher.cpp)
target_link_libraries(opencv_stitcher ${OpenCV_LIBS})
add_executable(og_opencv_stitcher ${PROJECT_SOURCE_DIR}/source/opencv_stitcher_og.cpp)
target_link_libraries(og_opencv_stitcher ${OpenCV_LIBS})