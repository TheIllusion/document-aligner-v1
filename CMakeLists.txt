cmake_minimum_required(VERSION 3.5)
project(document_aligner_v1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package (OpenCV REQUIRED)
include_directories ( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp GfttCornerDetector.h GfttCornerDetector.cpp LsdLineDetector.h LsdLineDetector.cpp SelectSquare.h SelectSquare.cpp)
add_executable(document_aligner_v1 ${SOURCE_FILES})

target_link_libraries(document_aligner_v1 ${OpenCV_LIBS})