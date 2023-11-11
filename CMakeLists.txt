# MIT License
# 
# Copyright (c) 2023 Mihail Mladenov
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


cmake_minimum_required(VERSION 3.5) 

project(web-lightning)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(
        GLOB
        WL_SOURCES 
        "src/common/*.cpp"
        "src/main_surface/*.cpp"
        "src/gpu_api/*.cpp"
        "src/algebra/*.cpp"
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_SDL=2 -s OFFSCREEN_FRAMEBUFFER=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
endif()

set(WL_SOURCE_DIR "src")
add_library(${PROJECT_NAME} ${WL_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${WL_SOURCE_DIR})

file(GLOB EXAMPLES "examples/*.cpp")
foreach(EXAMPLE "${EXAMPLES}")
    cmake_path(GET EXAMPLE STEM EXAMPLE_NAME)
    configure_file("index.html" ${CMAKE_BINARY_DIR})
    add_executable(${EXAMPLE_NAME} "${EXAMPLE}")
    target_link_libraries(${EXAMPLE_NAME} ${PROJECT_NAME})
    target_include_directories(${EXAMPLE_NAME} PUBLIC ${WL_SOURCE_DIR})
endforeach()
