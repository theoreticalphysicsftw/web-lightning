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
        "src/rendering/*.cpp"
        "src/shaders/*.cpp"
        "src/embedded/*.cpp"
    )

file(
        GLOB
        LIBWEBP_SOURCES
        "extern/libwebp/src/dec/*.c"
        "extern/libwebp/src/dsp/*.c"
        "extern/libwebp/src/demux/*.c"
        "extern/libwebp/src/utils/*.c"
    )

file(
        GLOB
        LIZARD_SOURCES
        "extern/lizard/*.c"
        "extern/lizard/entropy/*.c"
        "extern/lizard/xxhash/*.c"
    )

if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "${USE_FLAGS} -sMIN_WEBGL_VERSION=2")
    set(USE_FLAGS "${USE_FLAGS} -sMAX_WEBGL_VERSION=2")
    set(USE_FLAGS "${USE_FLAGS} -sUSE_WEBGL2=1")
    set(USE_FLAGS "${USE_FLAGS} -sUSE_SDL=2")
    set(USE_FLAGS "${USE_FLAGS} -sOFFSCREEN_FRAMEBUFFER=1")
    set(USE_FLAGS "${USE_FLAGS} -sALLOW_MEMORY_GROWTH")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
endif()

set(WL_SOURCE_DIR "src")
add_library(${PROJECT_NAME} ${WL_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${WL_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC "extern")

target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:WL_DEBUG>)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Release>:WL_RELEASE>)

option(WL_USE_EMBEDDED_FONT "Embed default font into the library" ON)
if (WL_USE_EMBEDDED_FONT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WL_USE_EMBEDDED_FONT)
endif()

file(GLOB EXAMPLES "examples/*.cpp")
foreach(EXAMPLE ${EXAMPLES})
    cmake_path(GET EXAMPLE STEM EXAMPLE_NAME)
    configure_file("index.html" ${CMAKE_BINARY_DIR})
    add_executable(${EXAMPLE_NAME} "${EXAMPLE}")

    target_link_libraries(${EXAMPLE_NAME} ${PROJECT_NAME})
    target_include_directories(${EXAMPLE_NAME} PUBLIC ${WL_SOURCE_DIR})

    if(NOT (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten"))
    target_link_libraries(${EXAMPLE_NAME} SDL2-static)
    endif()

endforeach()


if(NOT (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten"))
    add_subdirectory("extern/SDL2")
    add_subdirectory("extern/Glad")
    target_link_libraries(${PROJECT_NAME} SDL2-static)
    target_link_libraries(${PROJECT_NAME} glad-es3)
endif()

add_library(stb "extern/stb.cpp")
add_library(libwebp ${LIBWEBP_SOURCES})
add_library(lizard ${LIZARD_SOURCES})
target_include_directories(libwebp PUBLIC "extern/libwebp")

target_link_libraries(${PROJECT_NAME} stb)
target_link_libraries(${PROJECT_NAME} libwebp)
target_link_libraries(${PROJECT_NAME} lizard)

if(WIN32)
    # DWMAPI for window transparency functions from DWM.
    target_link_libraries(${PROJECT_NAME} "Dwmapi.lib")
    # Remove console window.
    target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:/SUBSYSTEM:windows>")
    target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:/ENTRY:mainCRTStartup>")
    # Statically link MSVC.
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endif()

add_subdirectory("extern/plutosvg")
target_link_libraries(${PROJECT_NAME} plutosvg)