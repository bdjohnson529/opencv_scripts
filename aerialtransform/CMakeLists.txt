project(protocol)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# OpenCV
find_package(OpenCV 3.2.0 REQUIRED)

# Eigen
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
    /usr/include
    /usr/local/include/
	${PROJECT_SOURCE_DIR}/include
	/usr/local/include/eigen3
)

link_directories(
	/usr/lib
	/usr/local/lib
	${PROJECT_SOURCE_DIR}/lib
)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADER "${PROJECT_SOURCE_DIR}/include/*.h*")

add_library(coord STATIC ${SOURCES} ${HEADERS})

#add_executable(image_coordinates ${PROJECT_SOURCE_DIR}/test/image_coordinates.cpp)
#target_link_libraries(image_coordinates coord pthread)
add_executable(testPerspective ${PROJECT_SOURCE_DIR}/test/testPerspective.cpp)
target_link_libraries(testPerspective coord ${OpenCV_LIBS})