
#include "CppUTest/TestHarness.h"
#include "TestIntcode.hpp"
#include "intcode.h"
#include "sol.h"

TEST_GROUP(DAY_2)
{
};

TEST(DAY_2, TEST_SOL)
{
    CHECK_EQUAL(6327510, solve_2019_2_1("input/2.txt"));
    CHECK_EQUAL(4112, solve_2019_2_2("input/2.txt"));
}

// All of the programs tested here simply show that, given
// an input program, the intcode runner produces the correct
// ending state
TEST_GROUP_BASE(INTCODE_DAY2, IntcodeTest_t)
{
    void run_test(const int bufsize, long *buffer,
                  long *expected_end, int expected_last_pc)
    {
        IntcodeReturn_t retval = test_program(bufsize, buffer,
                                              0, NULL, 0, NULL);
        CHECK(!buffers_different(bufsize, expected_end, buffer));
        CHECK_EQUAL(expected_last_pc, retval.pc);
        CHECK_EQUAL(0, retval.input_used);
        CHECK_EQUAL(1, retval.halt);
    }
};

TEST(INTCODE_DAY2, TEST0)
{
    const int bufsize = 12;
    long buffer[bufsize] = {1,9,10,3,2,3,11,0,99,30,40,50};
    long expected_end[bufsize] = {3500,9,10,70,2,3,11,0,99,30,40,50};

    run_test(bufsize, buffer, expected_end, 8);
}

TEST(INTCODE_DAY2, TEST1)
{
    const int bufsize = 5;
    long buffer [bufsize] = {1,0,0,0,99};
    long expected_end[bufsize] = {2,0,0,0,99};

    run_test(bufsize, buffer, expected_end, 4);
}

TEST(INTCODE_DAY2, TEST2)
{
    const int bufsize = 5;
    long buffer[bufsize] = {2,3,0,3,99};
    long expected_end[bufsize] = {2,3,0,6,99};

    run_test(bufsize, buffer, expected_end, 4);
}

TEST(INTCODE_DAY2, TEST3)
{
    const int bufsize = 6;
    long buffer[bufsize] = {2,4,4,5,99,0};
    long expected_end[bufsize] = {2,4,4,5,99,9801};

    run_test(bufsize, buffer, expected_end, 4);
}

TEST(INTCODE_DAY2, TEST4)
{
    const int bufsize = 9;
    long buffer[bufsize] = {1,1,1,4,99,5,6,0,99};
    long expected_end[bufsize] = {30,1,1,4,2,5,6,0,99};

    run_test(bufsize, buffer, expected_end, 8);
}
