cmake_minimum_required (VERSION 3.7)
project (dl-cpp)
set (DL_CPP_VERSION_MAJOR 0)
set (DL_CPP_VERSION_MINOR 1)
set (DL_CPP_VERSION_PATCH 0)

# Version
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set (DL_CPP_VERSION ${DL_CPP_VERSION_MAJOR}.${DL_CPP_VERSION_MINOR}.${DL_CPP_VERSION_PATCH})

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Source
set (SRCS )

# add_library(dl-cpp STATIC ${SRCS})

# install (TARGETS dl-cpp DESTINATION lib)
install (DIRECTORY ${PROJECT_SOURCE_DIR}/include/daiselabs DESTINATION include)
