cmake_minimum_required(VERSION 3.5.1)
project(easysolve)
enable_language(CXX)

## library
## MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
## HDF5
#set(HDF5_PREFER_PARALLEL TRUE)
find_package(HDF5 REQUIRED)
include_directories(${HDF5_INCLUDE_DIRS})
message("HDF5 is parallel? " ${HDF5_IS_PARALLEL})

option(_use_omp "enable OpenMP" OFF)
if(${_use_omp})
    find_package ( OpenMP REQUIRED )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -D_USE_OMP")
    include_directories(${OpenMP_CXX_INCLUDE_DIRS})
endif()

## compiler
set(CMAKE_CXX_COMPILER g++)

## compiler flags
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a")
if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message("DEBUG mode ON!")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g3 -O0 -pedantic -Wall -Wextra -Weffc++")
elseif(CMAKE_BUILD_TYPE MATCHES "RELEASE")
    message("RELEASE mode ON!")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")
endif()

message("CXX FLAGS: ${CMAKE_CXX_FLAGS}")

## compile
file(GLOB SOURCES src/*.cpp)
add_executable(easysolve ${SOURCES})
target_link_libraries(easysolve PUBLIC ${MPI_LIBRARIES} ${HDF5_C_LIBRARIES} ${OpenMP_CXX_LIBRARIES})
