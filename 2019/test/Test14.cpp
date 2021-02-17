
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_14){};

TEST(DAY_14, TEST_SOL)
{
    long part1 = solve_2019_14_1("input/14.txt");
    CHECK_EQUAL(612880, part1);
    long part2 = solve_2019_14_2("input/14.txt");
    CHECK_EQUAL(2509120, part2);
}

TEST(DAY_14, TEST_0)
{
    long part1 = solve_2019_14_1("input/14_test0.txt");
    CHECK_EQUAL(31, part1);
}

TEST(DAY_14, TEST_1)
{
    long part1 = solve_2019_14_1("input/14_test1.txt");
    CHECK_EQUAL(165, part1);
}

TEST(DAY_14, TEST_2)
{
    long part1 = solve_2019_14_1("input/14_test2.txt");
    CHECK_EQUAL(13312, part1);
    long part2 = solve_2019_14_2("input/14_test2.txt");
    CHECK_EQUAL(82892753, part2);
}

TEST(DAY_14, TEST_3)
{
    long part1 = solve_2019_14_1("input/14_test3.txt");
    CHECK_EQUAL(180697, part1);
    long part2 = solve_2019_14_2("input/14_test3.txt");
    CHECK_EQUAL(5586022, part2);
}

TEST(DAY_14, TEST_4)
{
    long part1 = solve_2019_14_1("input/14_test4.txt");
    CHECK_EQUAL(2210736, part1);
    long part2 = solve_2019_14_2("input/14_test4.txt");
    CHECK_EQUAL(460664, part2);
}
