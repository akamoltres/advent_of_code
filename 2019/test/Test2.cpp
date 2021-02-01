
#include "CppUTest/TestHarness.h"
#include "TestIntcode.hpp"
#include "intcode.h"
#include "sol.h"

#include <string.h>

TEST_GROUP(DAY_2){};

TEST(DAY_2, TEST_SOL)
{
    CHECK_EQUAL(6327510, solve_2019_2_1("input/2.txt"));
    CHECK_EQUAL(4112, solve_2019_2_2("input/2.txt"));
}

// All of the programs tested here simply show that, given
// an input program, the intcode runner produces the correct
// ending state
TEST_GROUP_BASE(INTCODE_DAY2,
                IntcodeTest_t){void run_test(Intcode_t * program, const int program_length,
                                             long *expected_end, int expected_last_pc){
    IntcodeReturn_t retval = test_program(program, 0, NULL, 0, NULL);
CHECK(!buffers_different(program_length, expected_end, program->program));
CHECK_EQUAL(expected_last_pc, program->pc);
CHECK_EQUAL(0, retval.input_used);
CHECK_EQUAL(1, retval.halt);
}
}
;

TEST(INTCODE_DAY2, TEST0)
{
    const int program_length = 12;
    long initial_program[program_length] = {1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50};
    long expected_end[program_length] = {3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    run_test(&program, program_length, expected_end, 8);
}

TEST(INTCODE_DAY2, TEST1)
{
    const int program_length = 5;
    long initial_program[program_length] = {1, 0, 0, 0, 99};
    long expected_end[program_length] = {2, 0, 0, 0, 99};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    run_test(&program, program_length, expected_end, 4);
}

TEST(INTCODE_DAY2, TEST2)
{
    const int program_length = 5;
    long initial_program[program_length] = {2, 3, 0, 3, 99};
    long expected_end[program_length] = {2, 3, 0, 6, 99};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    run_test(&program, program_length, expected_end, 4);
}

TEST(INTCODE_DAY2, TEST3)
{
    const int program_length = 6;
    long initial_program[program_length] = {2, 4, 4, 5, 99, 0};
    long expected_end[program_length] = {2, 4, 4, 5, 99, 9801};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    run_test(&program, program_length, expected_end, 4);
}

TEST(INTCODE_DAY2, TEST4)
{
    const int program_length = 9;
    long initial_program[program_length] = {1, 1, 1, 4, 99, 5, 6, 0, 99};
    long expected_end[program_length] = {30, 1, 1, 4, 2, 5, 6, 0, 99};

    Intcode_t program;
    program.pc = 0;
    program.relative_base = 0;
    memcpy(program.program, initial_program, program_length * sizeof(long));

    run_test(&program, program_length, expected_end, 8);
}
