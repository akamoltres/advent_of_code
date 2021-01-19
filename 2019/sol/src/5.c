
#include "intcode.h"

#include <stdio.h>

// Returns 0 if successful
long solve_2019_5(char *input_filename, int input)
{
    const int bufsize = 1000;
    long buffer[bufsize];

    const int input_bufsize = 1;
    long input_buffer[input_bufsize];
    input_buffer[0] = input;

    int program_length = read_intcode(bufsize, buffer, input_filename);
    if(program_length == -1)
    {
        return -1;
    }

    IntcodeReturn_t retval = {
        .pc = 0,
        .input_used = 0,
        .retval = 0,
        .halt = 0
    };

    int inputs_used = 0;
    do {
        retval = run_intcode(program_length, bufsize, buffer,
                             input_bufsize - inputs_used, input_buffer + inputs_used,
                             retval.pc);
        inputs_used += retval.input_used;
    } while(buffer[retval.pc] != 99);
    if(retval.halt == -1)
    {
        return -1;
    }

    return retval.retval;
}

// Returns 0 if successful
/*int solve_2019_5_2(char *input_filename)
{
    const int bufsize = 1000;
    long buffer[bufsize];

    const int input_bufsize = 1;
    int input_buffer[input_bufsize];
    input_buffer[0] = 5;

    int program_length = read_intcode(bufsize, buffer, input_filename);
    if(program_length == -1)
    {
        return -1;
    }

    IntcodeReturn_t retval = run_intcode(program_length, bufsize, buffer, input_bufsize, input_buffer);
    if(retval.halt == -1)
    {
        return -1;
    }

    return 0;
}*/

/*int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}*/
