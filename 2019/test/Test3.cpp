
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_3)
{
};

TEST(DAY_3, TEST_SOL)
{
    CHECK_EQUAL(557, solve_2019_3_1("input/3.txt"));
    CHECK_EQUAL(56410, solve_2019_3_2("input/3.txt"));
};

TEST(DAY_3, TEST_0)
{
    CHECK_EQUAL(6, solve_2019_3_1("input/3_test0.txt"));
    CHECK_EQUAL(30, solve_2019_3_2("input/3_test0.txt"));
}

TEST(DAY_3, TEST_1)
{
    CHECK_EQUAL(159, solve_2019_3_1("input/3_test1.txt"));
    CHECK_EQUAL(610, solve_2019_3_2("input/3_test1.txt"));
}

TEST(DAY_3, TEST_2)
{
    CHECK_EQUAL(135, solve_2019_3_1("input/3_test2.txt"));
    CHECK_EQUAL(410, solve_2019_3_2("input/3_test2.txt"));
}
