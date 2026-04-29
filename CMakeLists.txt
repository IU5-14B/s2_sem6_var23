cmake_minimum_required(VERSION 3.10)

project(MuseumProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(museum_app
    src/main.cpp
    src/MuseumItem.cpp
    src/Exhibit.cpp
    src/Hall.cpp
    src/Catalog.cpp
    src/Guide.cpp
    src/Visitor.cpp
    src/Museum.cpp
)

target_include_directories(museum_app PRIVATE include)
target_compile_options(museum_app PRIVATE -Wall -Wextra)
