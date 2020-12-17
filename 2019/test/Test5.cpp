
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_5)
{
};

TEST(DAY_5, TEST_SOL)
{
    CHECK_EQUAL(4511442, solve_2019_5("input/5.txt", 1));
    CHECK_EQUAL(12648139, solve_2019_5("input/5.txt", 5));
};
