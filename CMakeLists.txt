cmake_minimum_required (VERSION 3.0)

project(crect VERSION 0.1 LANGUAGES CXX C)

include(GNUInstallDirs)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
add_library(boost INTERFACE IMPORTED)

set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

enable_testing()

add_subdirectory(Test)
include_directories(src/crect)