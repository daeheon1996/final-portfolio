
cmake_minimum_required(VERSION 3.16)

project(IndustrialDefectInspection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

# Main Inspection Program
add_executable(inspection
    src/main.cpp
    src/DefectInspector.cpp
)

# OpenCV Include Directories
target_include_directories(inspection PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(inspection PRIVATE ${CMAKE_SOURCE_DIR}/include)

# OpenCV Libraries
target_link_libraries(inspection PRIVATE ${OpenCV_LIBS})
