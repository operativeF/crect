cmake_minimum_required (VERSION 3.0)

project(crect VERSION 0.1 LANGUAGES CXX C)

include(GNUInstallDirs)

include_directories("C:/dev/tmp/include")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Test)
include_directories(src/crect)