
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_15){};

TEST(DAY_15, TEST_SOL)
{
    long part1 = solve_2019_15_1("input/15.txt");
    CHECK_EQUAL(266, part1);
}
