
#include "intcode.h"

#include <stdio.h>

// Returns 0 if successful
int part1(char *input_filename)
{
    const int bufsize = 500;
    int buffer[bufsize];

    const int input_bufsize = 1;
    int input_buffer[input_bufsize];
    input_buffer[0] = 1;

    int program_length = read_intcode(bufsize, buffer, input_filename);
    if(program_length == -1)
    {
        return -1;
    }

    int retval = run_intcode(program_length, bufsize, buffer, input_bufsize, input_buffer);
    if(retval == -1)
    {
        return -1;
    }

    return 0;
}

// Returns 0 if successful
int part2(char *input_filename)
{
    const int bufsize = 1000;
    int buffer[bufsize];

    const int input_bufsize = 1;
    int input_buffer[input_bufsize];
    input_buffer[0] = 5;

    int program_length = read_intcode(bufsize, buffer, input_filename);
    if(program_length == -1)
    {
        return -1;
    }

    int retval = run_intcode(program_length, bufsize, buffer, input_bufsize, input_buffer);
    if(retval == -1)
    {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
