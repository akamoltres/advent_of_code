
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_8)
{
};

TEST(DAY_8, TEST_SOL)
{
    CHECK_EQUAL(2318, solve_2019_8_1("input/8.txt", 25, 6));

    const char *part2_sol = "011001001011110011001110010010100101000010010100101001011110111001000011100111101001010000100001001010010100101000010010100101001010010100000110011100";
    char *part2_retval = solve_2019_8_2("input/8.txt", 25, 6);
    STRCMP_EQUAL(part2_sol, part2_retval);
    free(part2_retval);
}

TEST(DAY_8, TEST_0)
{
    CHECK_EQUAL(1, solve_2019_8_1("input/8_test0.txt", 3, 2));
}

TEST(DAY_8, TEST_1)
{
    const char *sol = "0110";
    char *retval = solve_2019_8_2("input/8_test1.txt", 2, 2);
    STRCMP_EQUAL(sol, retval);
    free(retval);
}
