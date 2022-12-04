cmake_minimum_required(VERSION 2.8)
project( wingame )
enable_language(CXX)


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")
if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message("DEBUG mode ON!")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g3 -O0 -pedantic -Wall -Wextra -Weffc++")
elseif(CMAKE_BUILD_TYPE MATCHES "RELEASE")
    message("RELEASE mode ON!")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")
endif()

message("CXX FLAGS: ${CMAKE_CXX_FLAGS}")


file(GLOB SOURCES src/*.cpp)
add_executable( wingame ${SOURCES} )
