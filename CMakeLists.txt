cmake_minimum_required(VERSION 3.5)

project(XMLWriter LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Qt/6.0.2/msvc2019_64")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_executable(XMLWriter opendrivedefinitions.h t_opendrive.cpp opendrivexmlwriter.cpp main.cpp)

target_link_libraries(XMLWriter PRIVATE Qt6::Core)
