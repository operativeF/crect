
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS REQUIRED unit_test_framework)
add_library(boost INTERFACE IMPORTED)

set_property(TARGET boost PROPERTY
    INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

enable_testing()

add_executable(main_test
    async_queue.test.cpp
    clock.test.cpp
    job_resource_transforms.test.cpp
    job_comparisons.test.cpp
    pend_clear.test.cpp
    unique.test.cpp
)

target_compile_features(main_test
    PRIVATE cxx_std_20
)

target_include_directories(main_test
    PRIVATE ${Boost_INCLUDE_DIR}
    PRIVATE ../
    PRIVATE ../src
    PRIVATE ../deps/kvasir/mpl/src
)

target_link_directories(main_test
    PRIVATE ${Boost_LIBRARY_DIR}
)

target_link_libraries(main_test
    PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(NAME test COMMAND main_test)