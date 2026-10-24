cmake_minimum_required(VERSION 3.21)

project(CpuRenderer LANGUAGES CXX)

set(LOG_LEVEL "INFO" CACHE STRING "Compile-time spdlog level")
string(TOUPPER "${LOG_LEVEL}" LOG_LEVEL)
set(SPDLOG_LEVELS DEBUG INFO WARN ERROR OFF)
if(NOT LOG_LEVEL IN_LIST SPDLOG_LEVELS)
    message(FATAL_ERROR "Invalid LOG_LEVEL: ${LOG_LEVEL}")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog CONFIG REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/image
    ${CMAKE_CURRENT_SOURCE_DIR}/include/log
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shader
    ${CMAKE_CURRENT_SOURCE_DIR}/include/window
)
set(SOURCE_FILES
    source/main.cpp
    source/image/TgaImage.cpp
    source/log/Logger.cpp
    source/shader/VertexShader.cpp
    source/window/Window.cpp)
add_executable(CpuRenderer ${SOURCE_FILES})
target_compile_definitions(CpuRenderer PRIVATE
    NOMINMAX
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_${LOG_LEVEL}
)
target_link_libraries(CpuRenderer PRIVATE spdlog::spdlog)