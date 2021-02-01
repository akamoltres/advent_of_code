
#include "intcode.h"

#include <assert.h>
#include <string.h>

#define NUM_AMPS 5
#define LOOP_OFFSET 5

static long run_amplifiers(Intcode_t *program, long phase_settings[NUM_AMPS])
{
    int previous_output = 0;

    for (int amp = 0; amp < NUM_AMPS; ++amp)
    {
        Intcode_t amp_program;
        memcpy(&amp_program, program, sizeof(Intcode_t));

        int num_inputs = 2;
        long input_buffer[2] = {phase_settings[amp], previous_output};

        // intcode program isn't running through to halt, but it doesn't matter
        // since we're getting the output we need
        IntcodeReturn_t retval = run_intcode(&amp_program, num_inputs, input_buffer);
        previous_output = retval.retval;
    }

    return previous_output;
}

static long try_phase_settings(int depth, long phase_settings[NUM_AMPS], Intcode_t *program)
{
    if (depth == NUM_AMPS)
    {
        return run_amplifiers(program, phase_settings);
    }

    int max_signal = 0;

    for (int i = 0; i < NUM_AMPS; ++i)
    {
        int already_used = 0;
        for (int j = 0; j < depth; ++j)
        {
            if (phase_settings[j] == i)
            {
                already_used = 1;
                break;
            }
        }

        if (!already_used)
        {
            phase_settings[depth] = i;
            long max_with_phase = try_phase_settings(depth + 1, phase_settings, program);
            if (max_with_phase > max_signal)
            {
                max_signal = max_with_phase;
            }
        }
    }

    return max_signal;
}

long solve_2019_7_1(char const *input_filename)
{
    Intcode_t program;

    int program_length = read_intcode(&program, input_filename);
    assert(program_length != -1);

    long phase_settings[NUM_AMPS] = {0};
    return try_phase_settings(0, phase_settings, &program);
}

static long run_amplifiers_loop(Intcode_t *program, long phase_settings[NUM_AMPS])
{
    int previous_loop_end = 0;
    IntcodeReturn_t retval = {
        .input_used = 0,
        .retval = 0,
        .halt = 0,
    };

    Intcode_t amp_programs[NUM_AMPS];
    for (int amp = 0; amp < NUM_AMPS; ++amp)
    {
        memcpy(&amp_programs[amp], program, sizeof(Intcode_t));
    }

    int first_iteration = 1;
    do
    {
        int previous_output = previous_loop_end;

        for (int amp = 0; amp < NUM_AMPS; ++amp)
        {
            int num_inputs = 1;
            long input_buffer[2] = {previous_output, previous_output};

            if (first_iteration)
            {
                num_inputs = 2;
                input_buffer[0] = phase_settings[amp];
            }

            retval = run_intcode(&amp_programs[amp], num_inputs, input_buffer);

            previous_output = retval.retval;
        }

        first_iteration = 0;

        if (!retval.halt)
        {
            previous_loop_end = previous_output;
        }
    } while (!retval.halt);

    return previous_loop_end;
}

static long try_phase_settings_loop(int depth, long phase_settings[NUM_AMPS], Intcode_t *program)
{
    if (depth == NUM_AMPS)
    {
        return run_amplifiers_loop(program, phase_settings);
    }

    long max_signal = 0;

    for (int i = LOOP_OFFSET; i < LOOP_OFFSET + NUM_AMPS; ++i)
    {
        int already_used = 0;
        for (int j = 0; j < depth; ++j)
        {
            if (phase_settings[j] == i)
            {
                already_used = 1;
                break;
            }
        }

        if (!already_used)
        {
            phase_settings[depth] = i;
            long max_with_phase = try_phase_settings_loop(depth + 1, phase_settings, program);
            if (max_with_phase > max_signal)
            {
                max_signal = max_with_phase;
            }
        }
    }

    return max_signal;
}

long solve_2019_7_2(char const *input_filename)
{
    Intcode_t program;

    int program_length = read_intcode(&program, input_filename);
    assert(program_length != -1);

    long phase_settings[NUM_AMPS] = {0};
    return try_phase_settings_loop(0, phase_settings, &program);
}
