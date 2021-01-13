
#include <assert.h>
#include <string.h>

#include "CppUTest/TestHarness.h"
#include "TestIntcode.hpp"
#include "intcode.h"
#include "sol.h"

TEST_GROUP(DAY_5)
{
};

TEST(DAY_5, TEST_SOL)
{
    CHECK_EQUAL(4511442, solve_2019_5("input/5.txt", 1));
    CHECK_EQUAL(12648139, solve_2019_5("input/5.txt", 5));
};

TEST_GROUP_BASE(INTCODE_DAY5, IntcodeTest_c)
{
    // Tests an intcode program with the provided inputs
    // and validates against the provided outputs
    void test_program(const int bufsize,
                      int *buffer,
                      int input_count,
                      int *input_buffer,
                      int expected_output_count,
                      int *output_buffer)
    {
        IntcodeReturn_t retval;
        memset(&retval, 0, sizeof(IntcodeReturn_t));
        int num_inputs_used = 0;
        int output_count = 0;
        do {
            retval = run_intcode(bufsize,
                                 bufsize,
                                 buffer,
                                 input_count,
                                 input_buffer + num_inputs_used,
                                 retval.pc);
            if(!retval.halt)
            {
                num_inputs_used += retval.input_used;
                assert(num_inputs_used <= input_count);
                CHECK_EQUAL(output_buffer[output_count], retval.retval);
                assert(++output_count <= expected_output_count);
            }
        } while(!retval.halt);
        CHECK_EQUAL(expected_output_count, output_count);
    }

    // Tests an intcode program that has a single input
    // and a single output
    void test_with_input(const int bufsize,
                         int *buffer,
                         int expected_input,
                         int expected_output)
    {
        const int input_count = 1;
        int input_buffer[input_count] = {expected_input};
        const int output_count = 1;
        int output_buffer[output_count] = {expected_output};
        test_program(bufsize,
                     buffer,
                     input_count,
                     input_buffer,
                     output_count,
                     output_buffer);
    }
};

// Tests a program that outputs whatever it gets
// as input, then halts
TEST(INTCODE_DAY5, TEST0)
{
    const int bufsize = 5;
    int buffer[bufsize] = {3,0,4,0,99};
    int expected_end[bufsize] = {1984,0,4,0,99};
    const int input_count = 1;
    int input_buffer[input_count] = {1984};
    const int output_count = input_count;
    int *output_buffer = input_buffer;

    test_program(bufsize,
                 buffer,
                 input_count,
                 input_buffer,
                 output_count,
                 output_buffer);
    CHECK(!buffers_different(bufsize, expected_end, buffer));
};

// Tests a program that tests parameter modes by constructing
// a multiply instruction that constructs the halt condition
TEST(INTCODE_DAY5, TEST1)
{
    const int bufsize = 5;
    int buffer[bufsize] = {1002,4,3,4,33};
    int expected_end[bufsize] = {1002,4,3,4,99};

    test_program(bufsize,
                 buffer,
                 0,
                 NULL,
                 0,
                 NULL);
    CHECK(!buffers_different(bufsize, expected_end, buffer));
}

// Tests a program with a negative integer
TEST(INTCODE_DAY5, TEST2)
{
    const int bufsize = 5;
    int buffer[bufsize] = {1101,100,-1,4,0};
    int expected_end[bufsize] = {1101,100,-1,4,99};

    test_program(bufsize,
                 buffer,
                 0,
                 NULL,
                 0,
                 NULL);
    CHECK(!buffers_different(bufsize, expected_end, buffer));
}

// Tests a program that uses position mode to check if the input
// is equal to 8 or not
TEST(INTCODE_DAY5, TEST3)
{
    const int bufsize = 11;
    int buffer[bufsize];
    int program_start[bufsize] = {3,9,8,9,10,9,4,9,99,-1,8};

    // Test 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 8, 1);

    // Test less than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 7, 0);

    // Test greater than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 9, 0);
}

// Tests a program that uses position mode to check if the input
// is less than 8 or not
TEST(INTCODE_DAY5, TEST4)
{
    const int bufsize = 11;
    int buffer[bufsize];
    int program_start[bufsize] = {3,9,7,9,10,9,4,9,99,-1,8};

    // Test 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 8, 0);

    // Test less than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 7, 1);

    // Test greater than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 9, 0);
}

// Tests a program that uses immediate mode to check if the input
// is equal to 8 or not
TEST(INTCODE_DAY5, TEST5)
{
    const int bufsize = 9;
    int buffer[bufsize];
    int program_start[bufsize] = {3,3,1108,-1,8,3,4,3,99};

    // Test 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 8, 1);

    // Test less than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 7, 0);

    // Test greater than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 9, 0);
}

// Tests a program that uses immediate mode to check if the input
// is less than 8 or not
TEST(INTCODE_DAY5, TEST6)
{
    const int bufsize = 9;
    int buffer[bufsize];
    int program_start[bufsize] = {3,3,1107,-1,8,3,4,3,99};

    // Test 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 8, 0);

    // Test less than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 7, 1);

    // Test greater than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 9, 0);
}

// Tests a program that uses position mode with jumps
// to check if the input was zero or nonzero
TEST(INTCODE_DAY5, TEST7)
{
    const int bufsize = 16;
    int buffer[bufsize];
    int program_start[bufsize] = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};

    // Test 0
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 0, 0);

    // Test less than 0
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, -1, 1);

    // Test greater than 0
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 1, 1);
}

// Tests a program that uses immediate mode with jumps
// to check if the input was zero or nonzero
TEST(INTCODE_DAY5, TEST8)
{
    const int bufsize = 13;
    int buffer[bufsize];
    int program_start[bufsize] = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};

    // Test 0
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 0, 0);

    // Test less than 0
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, -1, 1);

    // Test greater than 0
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 1, 1);
}

// Tests a program with jumps to output 999 if the input is
// below 8, 1000 if it is equal to 8, or 1001 if it is greater
// than 8
TEST(INTCODE_DAY5, TEST9)
{
    const int bufsize = 47;
    int buffer[bufsize];
    int program_start[bufsize] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
                                  1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
                                  999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};

    // Test 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 8, 1000);

    // Test less than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 7, 999);

    // Test greater than 8
    memcpy(buffer, program_start, bufsize * sizeof(int));
    test_with_input(bufsize, buffer, 9, 1001);
}
