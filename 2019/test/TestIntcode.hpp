#pragma once

#include <assert.h>
#include <string.h>

#include "CppUTest/TestHarness.h"
#include "intcode.h"

typedef class IntcodeTest: public Utest
{
public:
    // returns 0 if identical, 1 if different
    int buffers_different(const int bufsize, long *expected, long *actual)
    {
        for(int i = 0; i < bufsize; ++i)
        {
            if(expected[i] != actual[i])
            {
                return 1;
            }
        }
        return 0;
    }

    // Tests an intcode program with the provided inputs
    // and validates against the provided outputs
    IntcodeReturn_t test_program(Intcode_t *program,
                                 int input_count,
                                 long *input_buffer,
                                 int expected_output_count,
                                 long *output_buffer)
    {
        int num_inputs_used = 0;
        int output_count = 0;

        while(1)
        {
            IntcodeReturn_t retval = run_intcode(program,
                                                 input_count - num_inputs_used,
                                                 input_buffer + num_inputs_used);

            if(!retval.halt)
            {
                num_inputs_used += retval.input_used;
                assert(num_inputs_used <= input_count);
                CHECK_EQUAL(output_buffer[output_count], retval.retval);
                assert(++output_count <= expected_output_count);
            }
            else
            {
                CHECK_EQUAL(expected_output_count, output_count);
                return retval;
            }
        }

        assert(0);
    }
} IntcodeTest_t;
