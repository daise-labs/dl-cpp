cmake_minimum_required (VERSION 3.8)
project (dl-cpp)

set (DL_CPP_VERSION_MAJOR 0)
set (DL_CPP_VERSION_MINOR 1)
set (DL_CPP_VERSION_PATCH 0)

# Version
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (DL_CPP_VERSION ${DL_CPP_VERSION_MAJOR}.${DL_CPP_VERSION_MINOR}.${DL_CPP_VERSION_PATCH})

# Additional cmake files
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set the C++ standard to C++11
set (CMAKE_CXX_STANDARD 11)

# Add the include dir in this project
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source dirs
set (SRC_BASE_DIR ${PROJECT_SOURCE_DIR}/src)
set (SRC_TEXT_DIR ${SRC_BASE_DIR}/text)

# Source
set (SRCS_TEXT ${SRC_TEXT_DIR}/scanner.cpp)
set (SRCS ${SRCS_TEXT})

add_library(dl-cpp STATIC ${SRCS})

# install (TARGETS dl-cpp DESTINATION lib)
install (DIRECTORY ${PROJECT_SOURCE_DIR}/include/daiselabs DESTINATION include)
