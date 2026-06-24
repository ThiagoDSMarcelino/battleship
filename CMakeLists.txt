cmake_minimum_required(VERSION 3.16)
project(battleship CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(battleship
    src/main.cpp
    src/game.cpp
    src/board.cpp
    src/ship.cpp
    src/utils.cpp
    src/enemy_strategy.cpp
    src/difficulty/easy_strategy.cpp
    src/difficulty/medium_strategy.cpp
    src/difficulty/hard_strategy.cpp
)

target_include_directories(battleship PRIVATE include)

target_compile_options(battleship PRIVATE -Wall -Wextra -Wpedantic)
