
#include "intcode.h"

#include <assert.h>
#include <stdio.h>

long solve_2019_5(char *input_filename, long input)
{
    Intcode_t program;

    const int input_bufsize = 1;
    long input_buffer[input_bufsize];
    input_buffer[0] = input;

    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    long final_output;
    while (1)
    {
        IntcodeReturn_t retval = run_intcode(&program, input_bufsize, input_buffer);

        if (retval.halt)
        {
            break;
        }

        final_output = retval.retval;
    }

    return final_output;
}
