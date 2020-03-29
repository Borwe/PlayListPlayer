cmake_minimum_required(VERSION 3.10)

project(main_gui_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main_gui_test main.cpp)
add_test(main_gui_test COMMAND main_gui_test)
