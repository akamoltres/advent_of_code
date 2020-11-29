
#include <stdio.h>

// Returns length of intcode program if success
// Returns -1 if file is invalid
// Returns -2 if buffer is not long enough
int read_intcode(const int bufsize, int *buffer, char *filename)
{
    int program_length = 0;

    FILE *fp;
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
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

        buffer[program_length++] = value;

        if(done)
        {
            fclose(fp);
            return program_length;
        }

        if(program_length == bufsize)
        {
            return -2;
        }
    }
}

int part1()
{
    const int bufsize = 200;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, "2.txt");

    return 0;
}

int main()
{
    printf("Part 1: %d\n", part1());
}
