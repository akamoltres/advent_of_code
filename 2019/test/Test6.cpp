
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_6)
{
};

TEST(DAY_6, TEST_SOL)
{
    CHECK_EQUAL(154386, solve_2019_6_1("input/6.txt"));
    CHECK_EQUAL(346, solve_2019_6_2("input/6.txt"));
}

TEST(DAY_6, TEST_0)
{
    CHECK_EQUAL(42, solve_2019_6_1("input/6_test0.txt"));
}

TEST(DAY_6, TEST_1)
{
    CHECK_EQUAL(4, solve_2019_6_2("input/6_test1.txt"));
}
