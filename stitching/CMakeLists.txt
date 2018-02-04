project(platform)
cmake_minimum_required(VERSION 2.6)

# OpenCV
find_package(OpenCV 3.2.0 REQUIRED)
find_package(Qt5Widgets)

# OpenCV
find_package(Eigen3 REQUIRED)

# Boost
find_package(Boost 1.54 COMPONENTS iostreams program_options filesystem)

# pcl
find_package(PCL 1.7 REQUIRED COMPONENTS common io)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

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
target_link_libraries(oksi_stitcher ${OpenCV_LIBS} boost_system boost_program_options boost_serialization)