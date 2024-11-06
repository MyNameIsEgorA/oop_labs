cmake_minimum_required(VERSION 3.8)

project(WorldOfShips CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    src/entities/ship_segment/ship_segment.cpp
    src/entities/ship/ship.cpp
    src/managers/ship_manager/ship_manager.cpp
    src/entities/field/field.cpp
    src/main.cpp
)

add_executable(Game ${SOURCES})