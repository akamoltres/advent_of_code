
#include <stdio.h>
#include <stdlib.h>

int read_input(char *input_filename, const int bufsize, int *buffer)
{
    int list_length = 0;

    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    while(fscanf(fp, "%d", buffer + list_length) != EOF)
    {
        list_length += 1;
    }

    fclose(fp);

    return list_length;
}

static int is_sum(int num, int *buffer, int preamble_start, int preamble_end)
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

int part1(char *input_filename, int preamble_length)
{
    const int bufsize = 2000;
    int buffer[bufsize];

    int list_length = read_input(input_filename, bufsize, buffer);
    if(list_length == -1)
    {
        return -1;
    }

    for(int i = preamble_length; i < list_length; ++i)
    {
        if(!is_sum(buffer[i], buffer, i - preamble_length, i))
        {
            return buffer[i];
        }
    }

    return -1;
}

int part2(char *input_filename)
{
    return -1;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Exactly 2 arguments (input file, preamble length) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1], atoi(argv[2])));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
