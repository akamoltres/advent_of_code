
#include "intcode.h"

#include <stdio.h>
#include <string.h>

void print_program(int program_length, int *buffer)
{
    if(program_length > 0)
    {
        printf("%d\n", buffer[0]);
        for(int i = 1; i < program_length; ++i)
        {
            printf(" %d", buffer[i]);
        }
        printf("\n");
    }
}

int read_intcode(const int bufsize, int *buffer, const char *filename)
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
        int negative = 0;
        while(fgets(c, 2, fp))
        {
            if(c[0] == '-')
            {
                negative = 1;
                continue;
            }

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

        buffer[program_length++] = value * (negative ? -1 : 1);

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

IntcodeReturn_t run_intcode(const int program_length, const int bufsize, int *buffer, const int input_length, int *input_buffer, int pc)
{
    IntcodeReturn_t state;
    memset(&state, 0, sizeof(IntcodeReturn_t));
    state.pc = pc;

    while(buffer[state.pc] != 99)
    {
        int mode1 = (buffer[state.pc] % 1000) / 100;
        int mode2 = (buffer[state.pc] % 10000) / 1000;
        switch(buffer[state.pc] % 100)
        {
            case 1: // add
            {
                int param1 = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                int param2 = (mode2 ? buffer[state.pc + 2] : buffer[buffer[state.pc + 2]]);
                buffer[buffer[state.pc + 3]] = param1 + param2;
                state.pc += 4;
                break;
            }
            case 2: // multiply
            {
                int param1 = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                int param2 = (mode2 ? buffer[state.pc + 2] : buffer[buffer[state.pc + 2]]);
                buffer[buffer[state.pc + 3]] = param1 * param2;
                state.pc += 4;
                break;
            }
            case 3: // input
            {
                if(state.input_used == input_length)
                {
                    state.halt = -1;
                    return state;
                }
                buffer[buffer[state.pc + 1]] = input_buffer[state.input_used++];
                state.pc += 2;
                break;
            }
            case 4: // output
            {
                state.retval = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                state.pc += 2;
                return state;
            }
            case 5: // jump if true
            {
                int param1 = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                int param2 = (mode2 ? buffer[state.pc + 2] : buffer[buffer[state.pc + 2]]);
                if(param1)
                {
                    state.pc = param2;
                }
                else
                {
                    state.pc += 3;
                }
                break;
            }
            case 6: // jump if false
            {
                int param1 = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                int param2 = (mode2 ? buffer[state.pc + 2] : buffer[buffer[state.pc + 2]]);
                if(!param1)
                {
                    state.pc = param2;
                }
                else
                {
                    state.pc += 3;
                }
                break;
            }
            case 7: // less than
            {
                int param1 = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                int param2 = (mode2 ? buffer[state.pc + 2] : buffer[buffer[state.pc + 2]]);
                buffer[buffer[state.pc + 3]] = (param1 < param2);
                state.pc += 4;
                break;
            }
            case 8: // equals
            {
                int param1 = (mode1 ? buffer[state.pc + 1] : buffer[buffer[state.pc + 1]]);
                int param2 = (mode2 ? buffer[state.pc + 2] : buffer[buffer[state.pc + 2]]);
                buffer[buffer[state.pc + 3]] = (param1 == param2);
                state.pc += 4;
                break;
            }
            default:
            {
                state.halt = -1;
                return state;
            }
        }

        if(state.pc >= bufsize)
        {
            state.halt = -1;
            return state;
        }
    }

    state.halt = 1;

    return state;
}
