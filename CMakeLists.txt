cmake_minimum_required (VERSION 3.0)

project(crect VERSION 0.1 LANGUAGES CXX C)

include(GNUInstallDirs)

include_directories("C:/dev/tmp/include")

add_subdirectory(Test)
include_directories(src/crect)