#pragma once

#include "CppUTest/TestHarness.h"

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
} IntcodeTest_c;
