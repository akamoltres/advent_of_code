
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_14) {};

TEST(DAY_14, TEST_SOL)
{
    int part1 = solve_2019_14_1("input/14.txt");
    CHECK_EQUAL(612880, part1);
}

TEST(DAY_14, TEST_0)
{
    int part1 = solve_2019_14_1("input/14_test0.txt");
    CHECK_EQUAL(31, part1);
}

TEST(DAY_14, TEST_1)
{
    int part1 = solve_2019_14_1("input/14_test1.txt");
    CHECK_EQUAL(165, part1);
}

TEST(DAY_14, TEST_2)
{
    int part1 = solve_2019_14_1("input/14_test2.txt");
    CHECK_EQUAL(13312, part1);
}

TEST(DAY_14, TEST_3)
{
    int part1 = solve_2019_14_1("input/14_test3.txt");
    CHECK_EQUAL(180697, part1);
}

TEST(DAY_14, TEST_4)
{
    int part1 = solve_2019_14_1("input/14_test4.txt");
    CHECK_EQUAL(2210736, part1);
}
