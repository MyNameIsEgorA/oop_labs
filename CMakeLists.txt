cmake_minimum_required(VERSION 3.8)

project(WorldOfShips CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(Game ${SOURCES}
        src/managers/ability_manager/exceptions.h
        src/managers/game_manager/game_manager.cpp
        src/managers/game_manager/game_manager.h
        src/game/game.cpp)