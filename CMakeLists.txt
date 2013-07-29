cmake_minimum_required(VERSION 2.6)

PROJECT(mjpegDecoder) 

message( STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")

SET(SRC
mjpegDecoder.cpp
DumpFileSplitter.cpp
MyList.cpp
JpegToAvi.cpp
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
