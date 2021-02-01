
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_1){};

TEST(DAY_1, TEST_SOL)
{
    CHECK_EQUAL(3576689, solve_2019_1_1("input/1.txt"));
    CHECK_EQUAL(5362136, solve_2019_1_2("input/1.txt"));
}
