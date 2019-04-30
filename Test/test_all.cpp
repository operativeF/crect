#include <thread>

#include "async_queue.test.hpp"
#include "claim.test.hpp"
#include "clock.test.hpp"
#include "job_comparisons.test.hpp"
#include "job_resource_transforms.test.hpp"
#include "pend_clear.test.hpp"
#include "source_masking.test.hpp"
#include "unique.test.hpp"
//#include "vector_table.test.hpp"

int main()
{
    std::thread job1(job_comparisons_test::run);
    std::thread job2(job_resource_transforms_test::run);
    std::thread job3(async_queue_test::run);
    std::thread job4(pend_clear_test::run);
    std::thread job5(source_masking_test::run);
    std::thread job6(unique_test::run);
    std::thread job7(clock_test::run);

    job1.join();
    job2.join();
    job3.join();
    job4.join();
    job5.join();
    job6.join();
    job7.join();

    return 0;
}