
#include "intcode.h"

#include <stdio.h>

// Returns 0 if successful
int part1()
{
    const int bufsize = 500;
    int buffer[bufsize];

    const int input_bufsize = 1;
    int input_buffer[input_bufsize];
    input_buffer[0] = 1;

    int program_length = read_intcode(bufsize, buffer, "5.txt");
    if(program_length == -1)
    {
        return -1;
    }

    int retval = run_intcode(program_length, buffer, input_bufsize, input_buffer);
    if(retval == -1)
    {
        return -1;
    }

    return 0;
}

int main()
{
    printf("Part 1: %d\n", part1());
    return 0;
}
