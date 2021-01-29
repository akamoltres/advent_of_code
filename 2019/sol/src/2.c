
#include "intcode.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Returns -1 if failed
long solve_2019_2_1(char *input_filename)
{
    Intcode_t program;

    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    program.program[1] = 12;
    program.program[2] = 2;

    (void)run_intcode(&program, 0, NULL);

    return program.program[0];
}

// Returns -1 if failed
long solve_2019_2_2(char *input_filename)
{
    Intcode_t program;

    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    const int desired_output = 19690720;

    for (int noun = 0; noun < 100; ++noun)
    {
        for (int verb = 0; verb < 100; ++verb)
        {
            Intcode_t test_program;
            memcpy(&test_program, &program, sizeof(Intcode_t));
            test_program.program[1] = noun;
            test_program.program[2] = verb;
            (void)run_intcode(&test_program, 0, NULL);
            if (test_program.program[0] == desired_output)
            {
                return (noun * 100 + verb);
            }
        }
    }

    assert(0);
    return 0;
}
