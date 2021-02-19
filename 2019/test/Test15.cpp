
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_15){};

TEST(DAY_15, TEST_SOL)
{
    long part1 = solve_2019_15("input/15.txt", 1);
    CHECK_EQUAL(266, part1);
    long part2 = solve_2019_15("input/15.txt", 2);
    CHECK_EQUAL(274, part2);
}
