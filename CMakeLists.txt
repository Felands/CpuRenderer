cmake_minimum_required(VERSION 3.21)

project(CpuRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog CONFIG REQUIRED)



include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shader
    ${CMAKE_CURRENT_SOURCE_DIR}/include/window
)
set(SOURCE_FILES
    source/main.cpp
    source/shader/VertexShader.cpp
    source/window/Window.cpp)
add_executable(CpuRenderer ${SOURCE_FILES})
target_compile_definitions(CpuRenderer PRIVATE
    NOMINMAX
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO
)
target_link_libraries(CpuRenderer PRIVATE spdlog::spdlog)