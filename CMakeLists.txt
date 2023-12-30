cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
project(JsonParser)


set(EXECUTABLE_OUTPUT_PATH .build/../bin)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
include_directories(headers)

set(SOURCES src/JSONObject.cpp src/JSONParser.cpp src/main.cpp)



add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -std=c++20)
