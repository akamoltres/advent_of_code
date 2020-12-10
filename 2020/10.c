
#include <stdio.h>

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

void sort(int list_length, int *buffer)
{
    for(int i = 0; i < list_length; ++i)
    {
        for(int j = 0; j + 1 < list_length; ++j)
        {
            if(buffer[j] > buffer[j + 1])
            {
                int temp = buffer[j];
                buffer[j] = buffer[j + 1];
                buffer[j + 1] = temp;
            }
        }
    }
}

int part1(char *input_filename)
{
    const int bufsize = 1000;
    int buffer[bufsize];

    int list_length = read_input(input_filename, bufsize, buffer);
    if(list_length == -1)
    {
        return -1;
    }

    sort(list_length, buffer);

    int count[4] = {0};
    count[buffer[0]] += 1;
    for(int i = 1; i < list_length; ++i)
    {
        count[buffer[i] - buffer[i - 1]] += 1;
    }
    count[3] += 1;

    return (count[1] * count[3]);
}

int part2(char *input_filename)
{
    return -1;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    // printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
