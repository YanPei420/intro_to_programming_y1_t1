cmake_minimum_required(VERSION 3.13)

project(Game)

add_executable(Game game.c)

target_link_libraries(Game ncurses)