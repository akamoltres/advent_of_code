
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR_SIZE 1000

#define PATTERN_LENGTH 4
long pattern[PATTERN_LENGTH] = {0, 1, 0, -1};

int solve_2019_16_1(char const *input_filename, int phases)
{
    // read and validate input
    char input_buffer[MAX_STR_SIZE];
    memset(input_buffer, 0, MAX_STR_SIZE * sizeof(char));
    FILE *fp = fopen(input_filename, "r");
    fscanf(fp, "%s", input_buffer);
    fclose(fp);

    // gosh i sure hope we don't have buffer overruns
    int len = strlen(input_buffer);
    assert(len < MAX_STR_SIZE);

    // convert to ints
    long list1[MAX_STR_SIZE];
    memset(list1, 0, len * sizeof(long));
    for (int i = 0; i < len; ++i)
    {
        list1[i] = input_buffer[i] - '0';
    }

    // iterate through the phases
    for (int p = 0; p < phases; ++p)
    {
        long list2[MAX_STR_SIZE];
        memset(list2, 0, len * sizeof(long));
        for (int i = 0; i < len; ++i)
        {
            // build the pattern
            int applied_pattern[MAX_STR_SIZE];
            int idx = 0;
            bool first = true;
            while (idx < len)
            {
                for (int j = 0; j < PATTERN_LENGTH; ++j)
                {
                    for (int k = 0; k <= i; ++k)
                    {
                        if (first)
                        {
                            first = false;
                        }
                        else
                        {
                            applied_pattern[idx++] = pattern[j];
                            if (idx == len)
                            {
                                break;
                            }
                        }
                    }
                    if (idx == len)
                    {
                        break;
                    }
                }
            }

            // apply the pattern
            for (int j = 0; j < len; ++j)
            {
                list2[i] += list1[j] * applied_pattern[j];
            }
            if (list2[i] < 0)
            {
                list2[i] *= -1;
            }
            list2[i] %= 10;
        }
        memcpy(list1, list2, len * sizeof(long));
    }

    // build eight digit output
    int output = 0;
    for (int i = 0; i < 8; ++i)
    {
        output = (10 * output) + list1[i];
    }

    return output;
}
