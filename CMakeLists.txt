cmake_minimum_required(VERSION 3.8)

project(WorldOfShips CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Найти все исходные файлы в директории src
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# Добавить исполняемый файл
add_executable(Game ${SOURCES})