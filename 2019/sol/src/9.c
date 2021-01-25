
#include "intcode.h"

#include <assert.h>
#include <stdio.h>

long solve_2019_9(char *input_filename, long input)
{
    Intcode_t program;
    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    const int input_bufsize = 1;
    long input_buffer[input_bufsize];
    input_buffer[0] = input;

    long output;
    IntcodeReturn_t retval = run_intcode(&program, input_bufsize, input_buffer);
    output = retval.retval;
    assert(retval.halt == 0);

    retval = run_intcode(&program, 0, NULL);
    assert(retval.halt == 1);

    return output;
}
