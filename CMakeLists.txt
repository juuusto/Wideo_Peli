cmake_minimum_required(VERSION 3.10)

# set the project name
project(WIDEO_PELI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 2.4 COMPONENTS audio graphics window system REQUIRED)

set(SOURCES
	src/boost.cpp
	src/Grid.cpp
	src/main.cpp
	src/Map.cpp
	src/tar.cpp
)

add_executable(WIDEO_PELI ${SOURCES})

target_link_libraries(WIDEO_PELI
        PUBLIC
        sfml-graphics sfml-audio sfml-window sfml-system sfml-network 
)

target_include_directories(WIDEO_PELI
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)

