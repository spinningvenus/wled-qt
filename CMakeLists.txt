cmake_minimum_required(VERSION 3.22)
project(wled-qt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Network Widgets)
message(STATUS "Qt version: ${Qt6_VERSION}")

add_executable(wled-qt main.cpp mainwindow.cpp mainwindow.h controller.cpp controller.h networkmanager.cpp networkmanager.h)
target_link_libraries(wled-qt Qt6::Network Qt6::Widgets)