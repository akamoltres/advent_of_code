
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_SIZE 100
#define BUFSIZE 30000005

int get_input(char *input_filename, int *buffer)
{
    int start_length = 0;

    FILE *fp;
    fp = fopen(input_filename, "r");

    if(fp == NULL)
    {
        fclose(fp);
        return -1;
    }

    int done = 0;
    while(1)
    {
        int value = 0;
        char c[2];
        while(fgets(c, 2, fp))
        {
            if(c[0] == ',')
            {
                break;
            }

            if(c[0] == '\n' || feof(fp))
            {
                done = 1;
                break;
            }

            value = (value * 10) + (c[0] - '0');
        }

        buffer[start_length++] = value;

        if(done)
        {
            fclose(fp);
            return start_length;
        }

        if(start_length == INPUT_SIZE)
        {
            fclose(fp);
            return -1;
        }
    }
}

int solution(char *input_filename, int target_turn)
{
    int buffer[INPUT_SIZE];

    int start_length = get_input(input_filename, buffer);
    if(start_length == -1)
    {
        return -1;
    }

    int *last_used;
    last_used = malloc(BUFSIZE * sizeof(int));
    memset(last_used, 0, BUFSIZE * sizeof(int));

    int prev_num;

    for(int i = 1; i <= start_length; ++i)
    {
        last_used[buffer[i - 1]] = i;
        prev_num = buffer[i - 1];
    }

    for(int i = start_length + 1; i <= target_turn; ++i)
    {
        assert(prev_num < BUFSIZE);

        if(last_used[prev_num] == 0 || last_used[prev_num] == i - 1)
        {
            last_used[prev_num] = i - 1;
            prev_num = 0;
        }
        else
        {
            int prev_num_cache = prev_num;
            prev_num = (i - 1) - last_used[prev_num];
            last_used[prev_num_cache] = i - 1;
        }
    }

    free(last_used);

    return prev_num;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", solution(argv[1], 2020));
    printf("Part 2: %d\n", solution(argv[1], 30000000));

    return 0;
}
