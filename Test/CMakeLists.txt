project(testlib VERSION 0.1 LANGUAGES CXX)

add_executable(
    job_comp_test job_comparisons.test.cpp
)

add_executable(
    unique_test unique.test.cpp
)

add_executable(
    job_res_test job_resource_transforms.test.cpp
)

target_link_libraries(job_comp_test
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

target_link_libraries(unique_test
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

target_link_libraries(job_res_test
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(NAME job_res_test COMMAND job_res_test)
add_test(NAME job_comp_test COMMAND job_comp_test)
add_test(NAME uniquetest COMMAND uniquetest)