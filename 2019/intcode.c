
#include "intcode.h"

#include <stdio.h>

int read_intcode(const int bufsize, int *buffer, char *filename)
{
    int program_length = 0;

    FILE *fp;
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        return -1;
    }

    int done = 0;
    while(1)
    {
        int value = 0;
        char c[2];
        while(fgets(c, 2, fp))
        {
            if(c[0] == ',')
            {
                break;
            }

            if(c[0] == '\n' || feof(fp))
            {
                done = 1;
                break;
            }

            value = (value * 10) + (c[0] - '0');
        }

        buffer[program_length++] = value;

        if(done)
        {
            fclose(fp);
            return program_length;
        }

        if(program_length == bufsize)
        {
            return -2;
        }
    }
}

int run_intcode(const int program_length, int *buffer)
{
    int position = 0;

    while(buffer[position] != 99)
    {
        switch(buffer[position])
        {
        case 1: // add
            buffer[buffer[position + 3]] = buffer[buffer[position + 1]] + buffer[buffer[position + 2]];
            break;
        case 2: // multiply
            buffer[buffer[position + 3]] = buffer[buffer[position + 1]] * buffer[buffer[position + 2]];
            break;
        default:
            return -1;
        }

        position += 4;
    }

    return 0;
}
