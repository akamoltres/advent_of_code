
#include "intcode.h"
#include "io.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void print_program(int program_length, Intcode_t *program)
{
    if(program_length > 0)
    {
        printf("%ld\n", program->program[0]);
        for(int i = 1; i < program_length; ++i)
        {
            printf(" %ld", program->program[i]);
        }
        printf("\n");
    }
}

int read_intcode(Intcode_t *program, const char *filename)
{
    program->pc = 0;
    program->relative_base = 0;
    return read_csv_line(filename, program->program, INTCODE_BUFFER_SIZE);
}

IntcodeReturn_t run_intcode(Intcode_t *program, const int input_length, long *input_buffer)
{
    IntcodeReturn_t state;
    memset(&state, 0, sizeof(IntcodeReturn_t));

    long *intcode = program->program;

    while(intcode[program->pc] != 99)
    {
        long mode1 = (intcode[program->pc] % 1000) / 100;
        long mode2 = (intcode[program->pc] % 10000) / 1000;
        switch(intcode[program->pc] % 100)
        {
            case 1: // add
            {
                long param1 = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                long param2 = (mode2 ? intcode[program->pc + 2] : intcode[intcode[program->pc + 2]]);
                intcode[intcode[program->pc + 3]] = param1 + param2;
                program->pc += 4;
                break;
            }
            case 2: // multiply
            {
                int param1 = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                int param2 = (mode2 ? intcode[program->pc + 2] : intcode[intcode[program->pc + 2]]);
                intcode[intcode[program->pc + 3]] = param1 * param2;
                program->pc += 4;
                break;
            }
            case 3: // input
            {
                assert(state.input_used < input_length);
                intcode[intcode[program->pc + 1]] = input_buffer[state.input_used++];
                program->pc += 2;
                break;
            }
            case 4: // output
            {
                state.retval = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                program->pc += 2;
                return state;
            }
            case 5: // jump if true
            {
                int param1 = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                int param2 = (mode2 ? intcode[program->pc + 2] : intcode[intcode[program->pc + 2]]);
                if(param1)
                {
                    program->pc = param2;
                }
                else
                {
                    program->pc += 3;
                }
                break;
            }
            case 6: // jump if false
            {
                int param1 = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                int param2 = (mode2 ? intcode[program->pc + 2] : intcode[intcode[program->pc + 2]]);
                if(!param1)
                {
                    program->pc = param2;
                }
                else
                {
                    program->pc += 3;
                }
                break;
            }
            case 7: // less than
            {
                int param1 = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                int param2 = (mode2 ? intcode[program->pc + 2] : intcode[intcode[program->pc + 2]]);
                intcode[intcode[program->pc + 3]] = (param1 < param2);
                program->pc += 4;
                break;
            }
            case 8: // equals
            {
                int param1 = (mode1 ? intcode[program->pc + 1] : intcode[intcode[program->pc + 1]]);
                int param2 = (mode2 ? intcode[program->pc + 2] : intcode[intcode[program->pc + 2]]);
                intcode[intcode[program->pc + 3]] = (param1 == param2);
                program->pc += 4;
                break;
            }
            default:
            {
                // shouldn't get here
                assert(0);
            }
        }

        assert(program->pc < INTCODE_BUFFER_SIZE);
    }

    state.halt = 1;

    return state;
}
