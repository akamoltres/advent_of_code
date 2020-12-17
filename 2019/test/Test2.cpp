
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_2)
{
};

TEST(DAY_2, TEST_SOL)
{
    CHECK_EQUAL(6327510, solve_2019_2_1("input/2.txt"));
    CHECK_EQUAL(4112, solve_2019_2_2("input/2.txt"));
};
