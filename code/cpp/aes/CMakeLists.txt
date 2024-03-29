cmake_minimum_required(VERSION 3.24)
project(aes)

set(CMAKE_CXX_STANDARD 23)

# include_directories() è legacy code, meglio usare un target.
include_directories(include)
# L'utilizzo di file(GLOB ...) è altamente scoraggiato.
#TODO: fix src/core/aes_api.cpp; perché src/core/*.cpp non lo prende in considerazione?
file(GLOB SOURCES src/*.cpp src/logger/*.cpp include/math/*.hpp src/math/random/*.cpp
        src/graphics/*.cpp src/graphics/console/*.cpp src/graphics/gui/*.cpp
        src/core/*.cpp src/core/aes_api.cpp
        include/core/operations/*.hpp src/core/padding/*.cpp src/core/modes/*.cpp
        src/file_manager/*.cpp include/convert/aes_convert.hpp
        include/version/version.hpp src/arguments_parser/*.cpp include/defaults/defaults.hpp
        include/core/aes_all.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# ----------- SPDLOG BEGIN --------------------------------------------

add_subdirectory(external/spdlog)

target_include_directories(${PROJECT_NAME} PUBLIC external/spdlog/include)

target_link_directories(${PROJECT_NAME} PRIVATE external/spdlog/src)

# ----------- SPDLOG END --------------------------------------------

# IMGUI - GLFW - GLAD - OPENGL SETUP

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)

set(DEAR_IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

set(DEAR_IMGUI_SOURCE
        "external/imgui/imgui.cpp"
        "external/imgui/imgui_demo.cpp"
        "external/imgui/imgui_draw.cpp"
        "external/imgui/imgui_tables.cpp"
        "external/imgui/imgui_widgets.cpp")

add_library(ImGui ${DEAR_IMGUI_SOURCE}) #ImGui
target_include_directories(ImGui PUBLIC ${DEAR_IMGUI_INCLUDE_DIR})

# GLFW OpenGL3 backend

set(DEAR_IMGUI_GLFW_OGL3_BACKEND_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/external/imgui/backends")

set(DEAR_IMGUI_GLFW_OGL3_BACKEND_SOURCE
        "external/imgui/backends/imgui_impl_opengl3.cpp"
        "external/imgui/backends/imgui_impl_glfw.cpp")

add_library(DearImGUIGlfwOgl3Backend ${DEAR_IMGUI_GLFW_OGL3_BACKEND_SOURCE})
target_include_directories(DearImGUIGlfwOgl3Backend PUBLIC ${DEAR_IMGUI_GLFW_OGL3_BACKEND_INCLUDE_DIR})
target_link_libraries(DearImGUIGlfwOgl3Backend PUBLIC glfw ImGui)

find_package(OpenGL REQUIRED) # I think this should not be here! But without it, it doesn't work.

target_link_libraries(${PROJECT_NAME} PUBLIC
        glfw
        glad
        DearImGUIGlfwOgl3Backend
        ImGui
        OpenGL::GL)

# -------------------------------------------------------

# DOCOPT SETUP

add_subdirectory(external/docopt)
target_include_directories(${PROJECT_NAME} PUBLIC external/docopt)
target_link_libraries(${PROJECT_NAME} PRIVATE docopt)

# -------------------------------------------------------

# ----------- IMGUIFILEBROWSER BEGIN --------------------------------------------

target_include_directories(${PROJECT_NAME} PUBLIC external/imgui-filebrowser)

# ----------- IMGUIFILEBROWSER END ----------------------------------------------

# ----------- GOOGLETEST BEGIN --------------------------------------------

add_subdirectory(external/googletest)

target_include_directories(${PROJECT_NAME} PUBLIC external/googletest/googletest/include)

target_link_directories(${PROJECT_NAME} PRIVATE external/googletest/googletest/src)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest) # PUBLIC

# ----------- GOOGLETEST END ----------------------------------------------

# ----------- TEST DIRECTORY BEGIN --------------------------------------------

#include_directories(tests) #TODO: remove
#link_directories(tests) #TODO: remove

#enable_testing()

add_subdirectory(tests) #../aes/tests

#include_directories(tests)

#target_link_libraries(${PROJECT_NAME} aes_tests) #TODO: remove

# ----------- TEST DIRECTORY END ----------------------------------------------

# ----------- DOXYGEN BEGIN ----------------------------------------------

# first we can indicate the documentation build as an option and set it to ON by default
#[[option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    # build the docs
    BUILD_DOC()
endif()]]

find_package(Doxygen)

if (DOXYGEN_FOUND)
    set( DOXYGEN_OUTPUT_DIRECTORY doxygen )
    set( DOXYGEN_COLLABORATION_GRAPH YES )
    set( DOXYGEN_EXTRACT_ALL YES )
    set( DOXYGEN_CLASS_DIAGRAMS YES )
    set( DOXYGEN_HIDE_UNDOC_RELATIONS NO )
    set( DOXYGEN_HAVE_DOT YES )
    set( DOXYGEN_CLASS_GRAPH YES )
    set( DOXYGEN_CALL_GRAPH YES )
    set( DOXYGEN_CALLER_GRAPH YES )
    set( DOXYGEN_COLLABORATION_GRAPH YES )
    set( DOXYGEN_BUILTIN_STL_SUPPORT YES )
    set( DOXYGEN_EXTRACT_PRIVATE YES )
    set( DOXYGEN_EXTRACT_PACKAGE YES )
    set( DOXYGEN_EXTRACT_STATIC YES )
    set( DOXYGEN_EXTRACT_LOCALMETHODS YES )
    set( DOXYGEN_UML_LOOK YES )
    set( DOXYGEN_UML_LIMIT_NUM_FIELDS 50 )
    set( DOXYGEN_TEMPLATE_RELATIONS YES )
    set( DOXYGEN_DOT_GRAPH_MAX_NODES 100 )
    set( DOXYGEN_MAX_DOT_GRAPH_DEPTH 0 )
    set( DOXYGEN_DOT_TRANSPARENT YES )

    doxygen_add_docs( doxygen ${RPP_PROJECT_SOURCE_DIR} )
else()
    message("Doxygen need to be installed to generate the doxygen documentation")
endif()

# ----------- DOXYGEN END ----------------------------------------------


