#pragma once

#include <assert.h>
#include <string.h>

#include "CppUTest/TestHarness.h"
#include "intcode.h"

typedef class IntcodeTest: public Utest
{
public:
    // returns 0 if identical, 1 if different
    int buffers_different(const int bufsize, int *expected, int *actual)
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
    IntcodeReturn_t test_program(const int bufsize,
                      int *buffer,
                      int input_count,
                      int *input_buffer,
                      int expected_output_count,
                      int *output_buffer)
    {
        IntcodeReturn_t retval;
        memset(&retval, 0, sizeof(IntcodeReturn_t));
        int num_inputs_used = 0;
        int output_count = 0;
        do {
            retval = run_intcode(bufsize,
                                 bufsize,
                                 buffer,
                                 input_count,
                                 input_buffer + num_inputs_used,
                                 retval.pc);
            if(!retval.halt)
            {
                num_inputs_used += retval.input_used;
                assert(num_inputs_used <= input_count);
                CHECK_EQUAL(output_buffer[output_count], retval.retval);
                assert(++output_count <= expected_output_count);
            }
        } while(!retval.halt);
        CHECK_EQUAL(expected_output_count, output_count);
        return retval;
    }
} IntcodeTest_c;
