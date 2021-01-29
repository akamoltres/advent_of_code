
#include "CppUTest/TestHarness.h"
#include "sol.h"

TEST_GROUP(DAY_7){};

TEST(DAY_7, TEST_SOL)
{
    CHECK_EQUAL(46014, solve_2019_7_1("input/7.txt"));
    CHECK_EQUAL(19581200, solve_2019_7_2("input/7.txt"));
}

TEST(DAY_7, TEST_0) { CHECK_EQUAL(43210, solve_2019_7_1("input/7_test0.txt")); }

TEST(DAY_7, TEST_1) { CHECK_EQUAL(54321, solve_2019_7_1("input/7_test1.txt")); }

TEST(DAY_7, TEST_2) { CHECK_EQUAL(65210, solve_2019_7_1("input/7_test2.txt")); }

TEST(DAY_7, TEST_3) { CHECK_EQUAL(139629729, solve_2019_7_2("input/7_test3.txt")); }

TEST(DAY_7, TEST_4) { CHECK_EQUAL(18216, solve_2019_7_2("input/7_test4.txt")); }
