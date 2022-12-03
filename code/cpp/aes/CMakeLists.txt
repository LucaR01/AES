cmake_minimum_required(VERSION 3.24)
project(aes)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(external/spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)

target_link_directories(${PROJECT_NAME} PRIVATE external/spdlog/src)
