cmake_minimum_required(VERSION 2.8)
project(ARMVO)
find_package(OpenCV REQUIRED)
include_directories($(OPENCV_INCLUDE_DIRS))

set(CMAKE_CXX_FLAGS "-msse4 ${CMAKE_CXX_FLAGS}")

#include_directories("/home/disk/tanjing/anaconda3/include/python3.7m")
#include_directories("/home/disk/tanjing/anaconda3/lib/python3.7/site-packages/numpy/core/include")

include_directories("/home/wuyuan/miniconda2/envs/wu_tsr/include/python3.6m")
include_directories("/home/wuyuan/miniconda2/envs/wu_tsr/lib/python3.6/site-packages/numpy/core/include")

include_directories("./src")

include_directories("./include")
include_directories("./")

add_subdirectory(pybind11)
FILE(GLOB SC_FILES "*.cpp" "./src/*.cpp" "./include/*.hpp")

link_directories("/home/disk/tanjing/anaconda3/lib")
pybind11_add_module(ARMVO ${SC_FILES})
target_link_libraries(ARMVO  ${OpenCV_LIBS})
