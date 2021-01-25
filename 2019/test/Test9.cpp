
#include "CppUTest/TestHarness.h"
#include "TestIntcode.hpp"
#include "sol.h"

#include <string.h>

TEST_GROUP(DAY_9)
{
};

TEST(DAY_9, TEST_SOL)
{
    CHECK_EQUAL(3235019597, solve_2019_9("input/9.txt", 1));
    CHECK_EQUAL(80274, solve_2019_9("input/9.txt", 2));
};

TEST_GROUP_BASE(INTCODE_DAY9, IntcodeTest_t)
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

// Tests a program that takes no inputs and produces a copy
// of itself as output
TEST(INTCODE_DAY9, TEST0)
{
    const int program_length = 16;
    long initial_program[program_length] = {109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};

    // TODO Go through intcode test and make a function to make this a one-liner
    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    (void) test_program(&program,
                        0, NULL,
                        program_length, initial_program);
}

// Tests a program that should output a 16-digit number
TEST(INTCODE_DAY9, TEST1)
{
    const int program_length = 8;
    long initial_program[program_length] = {1102,34915192,34915192,7,4,7,99,0};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    IntcodeReturn_t retval = run_intcode(&program, 0, NULL);
    CHECK(99999999999999 < retval.retval && retval.retval < 10000000000000000);
    CHECK_EQUAL(0, retval.halt);

    retval = run_intcode(&program, 0, NULL);
    CHECK_EQUAL(1, retval.halt);
}

// Tests a program that should output the large number
// in the middle of the program
TEST(INTCODE_DAY9, TEST2)
{
    const int program_length = 3;
    long initial_program[program_length] = {104,1125899906842624,99};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    (void) test_program(&program,
                        0, NULL,
                        1, initial_program + 1);
}
