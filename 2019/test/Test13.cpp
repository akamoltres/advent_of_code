
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_13){};

TEST(DAY_13, TEST_SOL)
{
    CHECK_EQUAL(216, solve_2019_13_1("input/13.txt"));
};
