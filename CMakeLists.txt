cmake_minimum_required(VERSION 3.8)

project(WorldOfShips CXX)

# add_subdirectory(src)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/entities/ship/ship.cpp
    src/managers/ship_manager/ship_manager.cpp
    src/entities/field/field.cpp
    src/main.cpp
)

add_executable(Game ${SOURCES})
# target_include_directories(WorldOfShips PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})