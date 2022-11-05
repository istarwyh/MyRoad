cmake_minimum_required(VERSION 3.23)
project(CTest)
ADD_SUBDIRECTORY(src bin)

set(CMAKE_C++_STANDARD 14)
#set(CMAKE_C_COMPILER "/Library/Developer/CommandLineTools/usr/bin/cc")
#set(CMAKE_CXX_COMPILER "/Library/Developer/CommandLineTools/usr/bin/c++")
set(CMAKE_INSTALL_PREFIX "/usr/local")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall")

MESSAGE(STATUS "This is a BINARY dir: " ${PROJECT_BINARY_DIR} )
MESSAGE(STATUS "This is a SOURCE dir: " ${PROJECT_SOURCE_DIR} )

INSTALL(FILES COPYRIGHT README.md DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/cmake)
INSTALL(PROGRAMS runHello.sh DESTINATION bin)
INSTALL(DIRECTORY doc/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/cmake)
