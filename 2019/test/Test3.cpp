
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
