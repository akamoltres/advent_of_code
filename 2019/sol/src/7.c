
#include "intcode.h"

#include <assert.h>
#include <string.h>

#define NUM_AMPS 5

static int run_amplifiers(int program_length, const int bufsize, int *program,
                          int phase_settings[NUM_AMPS])
{
    int previous_output = 0;

    for(int amp = 0; amp < NUM_AMPS; ++amp)
    {
        int amp_program[bufsize];
        memcpy(amp_program, program, bufsize * sizeof(int));

        int num_inputs = 2;
        int input_buffer[2] = {phase_settings[amp], previous_output};

        // intcode program isn't running through to halt, but it doesn't matter
        // since we're getting the output we need
        IntcodeReturn_t retval = run_intcode(program_length, bufsize, amp_program,
                                             num_inputs, input_buffer, 0);
        previous_output = retval.retval;
    }

    return previous_output;
}

static int try_phase_settings(int depth, int phase_settings[NUM_AMPS],
                              int program_length, const int bufsize, int *program)
{
    if(depth == NUM_AMPS)
    {
        return run_amplifiers(program_length, bufsize, program, phase_settings);
    }

    int max_signal = 0;

    for(int i = 0; i < NUM_AMPS; ++i)
    {
        int already_used = 0;
        for(int j = 0; j < depth; ++j)
        {
            if(phase_settings[j] == i)
            {
                already_used = 1;
                break;
            }
        }

        if(!already_used)
        {
            phase_settings[depth] = i;
            int max_with_phase = try_phase_settings(depth + 1, phase_settings,
                                                    program_length, bufsize, program);
            if(max_with_phase > max_signal)
            {
                max_signal = max_with_phase;
            }
        }
    }

    return max_signal;
}

int solve_2019_7_1(char const *input_filename)
{
    const int bufsize = 1000;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, input_filename);
    assert(program_length != -1);

    int phase_settings[NUM_AMPS] = {0};
    return try_phase_settings(0, phase_settings, program_length, bufsize, buffer);
}
