
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_16){};

TEST(DAY_16, TEST_SOL)
{
    int part1 = solve_2019_16_1("input/16.txt", 100);
    CHECK_EQUAL(22122816, part1);
}

TEST(DAY_16, TEST_0)
{
    int part1 = solve_2019_16_1("input/16_test0.txt", 0);
    CHECK_EQUAL(12345678, part1);

    part1 = solve_2019_16_1("input/16_test0.txt", 1);
    CHECK_EQUAL(48226158, part1);

    part1 = solve_2019_16_1("input/16_test0.txt", 2);
    CHECK_EQUAL(34040438, part1);

    part1 = solve_2019_16_1("input/16_test0.txt", 3);
    CHECK_EQUAL(3415518, part1);

    part1 = solve_2019_16_1("input/16_test0.txt", 4);
    CHECK_EQUAL(1029498, part1);
}

TEST(DAY_16, TEST_1)
{
    int part1 = solve_2019_16_1("input/16_test1.txt", 100);
    CHECK_EQUAL(24176176, part1);
}

TEST(DAY_16, TEST_2)
{
    int part1 = solve_2019_16_1("input/16_test2.txt", 100);
    CHECK_EQUAL(73745418, part1);
}

TEST(DAY_16, TEST_3)
{
    int part1 = solve_2019_16_1("input/16_test3.txt", 100);
    CHECK_EQUAL(52432133, part1);
}

TEST(DAY_16, TEST_4)
{
    int part2 = solve_2019_16_2("input/16_test4.txt", 100);
    CHECK_EQUAL(84462026, part2);
}

TEST(DAY_16, TEST_5)
{
    int part2 = solve_2019_16_2("input/16_test5.txt", 100);
    CHECK_EQUAL(78725270, part2);
}

TEST(DAY_16, TEST_6)
{
    int part2 = solve_2019_16_2("input/16_test6.txt", 100);
    CHECK_EQUAL(53553731, part2);
}
