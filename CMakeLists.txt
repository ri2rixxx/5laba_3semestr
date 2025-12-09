cmake_minimum_required(VERSION 3.10)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматический поиск SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Компиляторные флаги для Linux
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Исходные файлы
set(SOURCES
    main_graphical.cpp
    GraphicalMinesweeperGame.cpp
    MineField.cpp
    Cell.cpp
    MineCell.cpp
    SafeCell.cpp
)

# Графическая версия
add_executable(minesweeper
    ${SOURCES}
)

# Подключение SFML
target_link_libraries(minesweeper
    sfml-graphics
    sfml-window
    sfml-system
)

message(STATUS "Проект настроен для Linux")
message(STATUS "SFML найден: ${SFML_FOUND}")
