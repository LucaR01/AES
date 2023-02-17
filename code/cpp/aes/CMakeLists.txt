cmake_minimum_required(VERSION 3.24)
project(aes)

set(CMAKE_CXX_STANDARD 23)

# include_directories() è legacy code, meglio usare un target.
include_directories(include)
# L'utilizzo di file(GLOB ...) è altamente scoraggiato.
#TODO: fix src/core/aes_api.cpp; perché src/core/*.cpp non lo prende in considerazione?
file(GLOB SOURCES src/*.cpp src/logger/*.cpp src/math/*.cpp src/math/random/*.cpp
        src/graphics/*.cpp src/graphics/console/*.cpp src/core/*.cpp src/core/aes_api.cpp
        src/core/operations/*.cpp src/core/padding/*.cpp src/core/modes/*.cpp
        src/file_manager/*.cpp include/convert/aes_convert.hpp src/convert/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(external/spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)

target_link_directories(${PROJECT_NAME} PRIVATE external/spdlog/src)
