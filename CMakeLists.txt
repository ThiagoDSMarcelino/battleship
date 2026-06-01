cmake_minimum_required(VERSION 3.16)
project(battleship CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(battleship
    src/main.cpp
    src/Game.cpp
    src/Board.cpp
    src/Ship.cpp
    src/Utils.cpp
)

target_include_directories(battleship PRIVATE include)

target_compile_options(battleship PRIVATE -Wall -Wextra -Wpedantic)
