
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_4){};

TEST(DAY_4, TEST_SOL)
{
    CHECK_EQUAL(1716, solve_2019_4_1("input/4.txt"));
    CHECK_EQUAL(1163, solve_2019_4_2("input/4.txt"));
}
