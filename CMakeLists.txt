cmake_minimum_required(VERSION 3.30)

project(generate-sdf-data)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(freetype)

if (NOT "${CMAKE_GENERATOR}" MATCHES "Visual Studio")
  file(WRITE freetype/src/base/ftver.rc "")
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE freetype/include)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)