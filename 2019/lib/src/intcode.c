
#include "intcode.h"
#include "io.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void print_program(const int program_length, Intcode_t const * const program)
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

int read_intcode(Intcode_t *program, char const * const filename)
{
    program->pc = 0;
    program->relative_base = 0;
    return read_csv_line(filename, program->program, INTCODE_BUFFER_SIZE);
}

static long get_param(const int mode, const int offset, Intcode_t const * const program)
{
    long const * const intcode = program->program;

    // position mode
    if(mode == 0)
    {
        assert(program->pc + offset < INTCODE_BUFFER_SIZE);
        assert(intcode[program->pc + offset] < INTCODE_BUFFER_SIZE);
        return intcode[intcode[program->pc + offset]];
    }

    // immediate mode
    else if(mode == 1)
    {
        assert(program->pc + offset < INTCODE_BUFFER_SIZE);
        return intcode[program->pc + offset];
    }

    // relative mode
    else if(mode == 2)
    {
        assert(program->pc + offset < INTCODE_BUFFER_SIZE);
        assert(0 <= intcode[program->pc + offset] + program->relative_base &&
                    intcode[program->pc + offset] + program->relative_base < INTCODE_BUFFER_SIZE);
        return intcode[intcode[program->pc + offset] + program->relative_base];
    }

    // should never get here
    assert(0);
}

static long get_index(const int mode, const int offset, Intcode_t const * const program)
{
    long const * const intcode = program->program;

    assert(mode == 2 || mode == 0);
    long base = (mode == 2 ? program->relative_base : 0);

    assert(program->pc + offset < INTCODE_BUFFER_SIZE);
    assert(intcode[program->pc + offset] + base < INTCODE_BUFFER_SIZE);

    return intcode[program->pc + offset] + base;
}

IntcodeReturn_t run_intcode(Intcode_t *program, const int input_length, long *input_buffer)
{
    IntcodeReturn_t state;
    memset(&state, 0, sizeof(IntcodeReturn_t));

    long *intcode = program->program;

    while(intcode[program->pc] != 99)
    {
        int mode1 = (intcode[program->pc] % 1000) / 100;
        int mode2 = (intcode[program->pc] % 10000) / 1000;
        int mode3 = (intcode[program->pc] % 100000) / 10000;
        switch(intcode[program->pc] % 100)
        {
            case 1: // add
            {
                long param1 = get_param(mode1, 1, program);
                long param2 = get_param(mode2, 2, program);
                intcode[get_index(mode3, 3, program)] = param1 + param2;
                program->pc += 4;
                break;
            }
            case 2: // multiply
            {
                long param1 = get_param(mode1, 1, program);
                long param2 = get_param(mode2, 2, program);
                intcode[get_index(mode3, 3, program)] = param1 * param2;
                program->pc += 4;
                break;
            }
            case 3: // input
            {
                assert(state.input_used < input_length);
                intcode[get_index(mode1, 1, program)] = input_buffer[state.input_used++];
                program->pc += 2;
                break;
            }
            case 4: // output
            {
                state.retval = get_param(mode1, 1, program);
                program->pc += 2;
                return state;
            }
            case 5: // jump if true
            {
                long param1 = get_param(mode1, 1, program);
                long param2 = get_param(mode2, 2, program);
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
                long param1 = get_param(mode1, 1, program);
                long param2 = get_param(mode2, 2, program);
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
                long param1 = get_param(mode1, 1, program);
                long param2 = get_param(mode2, 2, program);
                intcode[get_index(mode3, 3, program)] = (param1 < param2);
                program->pc += 4;
                break;
            }
            case 8: // equals
            {
                long param1 = get_param(mode1, 1, program);
                long param2 = get_param(mode2, 2, program);
                intcode[get_index(mode3, 3, program)] = (param1 == param2);
                program->pc += 4;
                break;
            }
            case 9: // adjust relative base
            {
                long param = get_param(mode1, 1, program);
                program->relative_base += param;
                program->pc += 2;
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
