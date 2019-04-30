
enable_testing()

add_executable(main_test
    test_all.cpp
)

target_compile_features(main_test
    PRIVATE cxx_std_20
)

target_include_directories(main_test
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ../
    PRIVATE ../src
    PRIVATE ../deps/kvasir/mpl/src
)

add_test(NAME test COMMAND main_test)