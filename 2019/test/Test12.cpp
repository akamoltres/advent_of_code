
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_12){};

TEST(DAY_12, TEST_SOL)
{
    CHECK_EQUAL(7722, solve_2019_12_1("input/12.txt", 1000));
    CHECK_EQUAL(292653556339368, solve_2019_12_2("input/12.txt"));
}

TEST(DAY_12, TEST_0)
{
    CHECK_EQUAL(179, solve_2019_12_1("input/12_test0.txt", 10));
    CHECK_EQUAL(2772, solve_2019_12_2("input/12_test0.txt"));
}

TEST(DAY_12, TEST_1)
{
    CHECK_EQUAL(1940, solve_2019_12_1("input/12_test1.txt", 100));
    CHECK_EQUAL(4686774924, solve_2019_12_2("input/12_test1.txt"));
}
