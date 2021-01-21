
#include "CppUTest/TestHarness.h"
#include "TestIntcode.hpp"
#include "sol.h"

#include <string.h>

TEST_GROUP(DAY_5)
{
};

TEST(DAY_5, TEST_SOL)
{
    CHECK_EQUAL(4511442, solve_2019_5("input/5.txt", 1));
    CHECK_EQUAL(12648139, solve_2019_5("input/5.txt", 5));
};

TEST_GROUP_BASE(INTCODE_DAY5, IntcodeTest_t)
{
    // Tests an intcode program that has a single input
    // and a single output
    void test_with_input(Intcode_t *program,
                         long expected_input,
                         long expected_output)
    {
        const int input_count = 1;
        long input_buffer[input_count];
        input_buffer[0] = expected_input;

        const int output_count = 1;
        long output_buffer[output_count];
        output_buffer[0] = expected_output;

        (void) test_program(program,
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
    const int program_length = 5;
    long initial_program[program_length] = {3,0,4,0,99};
    long expected_end[program_length] = {1984,0,4,0,99};
    long input = 1984;
    long expected_output = input;

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    test_with_input(&program, input, expected_output);

    CHECK(!buffers_different(program_length, expected_end, program.program));
};

// Tests a program that tests parameter modes by constructing
// a multiply instruction that constructs the halt condition
TEST(INTCODE_DAY5, TEST1)
{
    const int program_length = 5;
    long initial_program[program_length] = {1002,4,3,4,33};
    long expected_end[program_length] = {1002,4,3,4,99};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    (void) test_program(&program, 0, NULL, 0, NULL);

    CHECK(!buffers_different(program_length, expected_end, program.program));
}

// Tests a program with a negative integer
TEST(INTCODE_DAY5, TEST2)
{
    const int program_length = 5;
    long initial_program[program_length] = {1101,100,-1,4,0};
    long expected_end[program_length] = {1101,100,-1,4,99};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    (void) test_program(&program, 0, NULL, 0, NULL);

    CHECK(!buffers_different(program_length, expected_end, program.program));
}

// Tests a program that uses position mode to check if the input
// is equal to 8 or not
TEST(INTCODE_DAY5, TEST3)
{
    const int program_length = 11;
    long initial_program[program_length] = {3,9,8,9,10,9,4,9,99,-1,8};

    Intcode_t program;

    // Test 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 8, 1);

    // Test less than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 7, 0);

    // Test greater than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 9, 0);
}

// Tests a program that uses position mode to check if the input
// is less than 8 or not
TEST(INTCODE_DAY5, TEST4)
{
    const int program_length = 11;
    long initial_program[program_length] = {3,9,7,9,10,9,4,9,99,-1,8};

    Intcode_t program;

    // Test 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 8, 0);

    // Test less than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 7, 1);

    // Test greater than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 9, 0);
}

// Tests a program that uses immediate mode to check if the input
// is equal to 8 or not
TEST(INTCODE_DAY5, TEST5)
{
    const int program_length = 9;
    long initial_program[program_length] = {3,3,1108,-1,8,3,4,3,99};

    Intcode_t program;

    // Test 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 8, 1);

    // Test less than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 7, 0);

    // Test greater than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 9, 0);
}

// Tests a program that uses immediate mode to check if the input
// is less than 8 or not
TEST(INTCODE_DAY5, TEST6)
{
    const int program_length = 9;
    long initial_program[program_length] = {3,3,1107,-1,8,3,4,3,99};

    Intcode_t program;

    // Test 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 8, 0);

    // Test less than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 7, 1);

    // Test greater than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 9, 0);
}

// Tests a program that uses position mode with jumps
// to check if the input was zero or nonzero
TEST(INTCODE_DAY5, TEST7)
{
    const int program_length = 16;
    long initial_program[program_length] = {3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};

    Intcode_t program;

    // Test 0
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 0, 0);

    // Test less than 0
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, -1, 1);

    // Test greater than 0
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 1, 1);
}

// Tests a program that uses immediate mode with jumps
// to check if the input was zero or nonzero
TEST(INTCODE_DAY5, TEST8)
{
    const int program_length = 13;
    long initial_program[program_length] = {3,3,1105,-1,9,1101,0,0,12,4,12,99,1};

    Intcode_t program;

    // Test 0
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 0, 0);

    // Test less than 0
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, -1, 1);

    // Test greater than 0
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 1, 1);
}

// Tests a program with jumps to output 999 if the input is
// below 8, 1000 if it is equal to 8, or 1001 if it is greater
// than 8
TEST(INTCODE_DAY5, TEST9)
{
    const int program_length = 47;
    long initial_program[program_length] = {3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,
                                            1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,
                                            999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99};

    Intcode_t program;

    // Test 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 8, 1000);

    // Test less than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 7, 999);

    // Test greater than 8
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));
    test_with_input(&program, 9, 1001);
}
