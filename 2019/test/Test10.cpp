
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_10)
{
};

TEST(DAY_10, TEST_SOL)
{
    CHECK_EQUAL(227, solve_2019_10_1("input/10.txt"));
    CHECK_EQUAL(604, solve_2019_10_2("input/10.txt"));
}

TEST(DAY_10, TEST_0)
{
    CHECK_EQUAL(8, solve_2019_10_1("input/10_test0.txt"));
}

TEST(DAY_10, TEST_1)
{
    CHECK_EQUAL(33, solve_2019_10_1("input/10_test1.txt"));
}

TEST(DAY_10, TEST_2)
{
    CHECK_EQUAL(35, solve_2019_10_1("input/10_test2.txt"));
}

TEST(DAY_10, TEST_3)
{
    CHECK_EQUAL(41, solve_2019_10_1("input/10_test3.txt"));
}

TEST(DAY_10, TEST_4)
{
    CHECK_EQUAL(210, solve_2019_10_1("input/10_test4.txt"));
    CHECK_EQUAL(802, solve_2019_10_2("input/10_test4.txt"));
}
