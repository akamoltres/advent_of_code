
#include <stdio.h>
#include <stdlib.h>

int read_input(char *input_filename, const int bufsize, long *buffer)
{
    int list_length = 0;

    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    while(fscanf(fp, "%ld", buffer + list_length) != EOF)
    {
        list_length += 1;
    }

    fclose(fp);

    return list_length;
}

static int is_sum(long num, long *buffer, int preamble_start, int preamble_end)
{
    for(int i = preamble_start; i < preamble_end; ++i)
    {
        for(int j = i + 1; j < preamble_end; ++j)
        {
            if(buffer[i] + buffer[j] == num)
            {
                return 1;
            }
        }
    }

    return 0;
}

// If retval == 0: returns the value of the first failing number
// If retval == 1: returns the index of the first failing number
long part1(char *input_filename, int preamble_length, int retval)
{
    const int bufsize = 2000;
    long buffer[bufsize];

    int list_length = read_input(input_filename, bufsize, buffer);
    if(list_length == -1)
    {
        return -1;
    }

    for(int i = preamble_length; i < list_length; ++i)
    {
        if(!is_sum(buffer[i], buffer, i - preamble_length, i))
        {
            if(retval == 0)
            {
                return buffer[i];
            }
            else if(retval == 1)
            {
                return i;
            }
            else
            {
                return -1;
            }
        }
    }

    return -1;
}

long part2(char *input_filename, int preamble_length)
{
    const int bufsize = 2000;
    long buffer[bufsize];
    long sums[bufsize];

    int list_length = read_input(input_filename, bufsize, buffer);
    if(list_length == -1)
    {
        return -1;
    }

    long invalid_index = part1(input_filename, preamble_length, 1);

    sums[0] = buffer[0];
    for(int i = 1; i < list_length; ++i)
    {
        sums[i] = sums[i - 1] + buffer[i];
    }

    for(int i = 0; i < invalid_index; ++i)
    {
        for(int j = i + 1; j < invalid_index; ++j)
        {
            if(sums[j] - sums[i] == buffer[invalid_index])
            {
                long min = buffer[i];
                long max = buffer[i];
                for(int k = i + 1; k < j; ++k)
                {
                    min = (min < buffer[k] ? min : buffer[k]);
                    max = (max > buffer[k] ? max : buffer[k]);
                }

                return (min + max);
            }
        }
    }

    return -1;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Exactly 2 arguments (input file, preamble length) required\n");
        return -1;
    }

    printf("Part 1: %ld\n", part1(argv[1], atoi(argv[2]), 0));
    printf("Part 2: %ld\n", part2(argv[1], atoi(argv[2])));

    return 0;
}
