
#include <stdio.h>

// Returns number of elements in file
// Returns -1 if buffer is too small
int get_input(int bufsize, int *buffer)
{
    int num_elements = 0;

    FILE *fp;
    fp = fopen("1.txt", "r");

    while(!feof(fp))
    {
        if(num_elements == bufsize)
        {
            return -1;
        }

        fscanf(fp, "%d", &buffer[num_elements++]);
    }

    fclose(fp);

    return num_elements;
}

// Returns -1 if failed unexpectedly
int part1()
{
    const int bufsize = 300;
    int buffer[bufsize];

    int num_elements = get_input(bufsize, buffer);
    if(num_elements == -1)
    {
        return -1;
    }

    for(int i = 0; i < num_elements; ++i)
    {
        for(int j = i + 1; j < num_elements; ++j)
        {
            if(buffer[i] + buffer[j] == 2020)
            {
                return (buffer[i] * buffer[j]);
            }
        }
    }

    return -1;
}

// Returns -1 if failed unexpectedly
int part2()
{
    const int bufsize = 300;
    int buffer[bufsize];

    int num_elements = get_input(bufsize, buffer);
    if(num_elements == -1)
    {
        return -1;
    }

    for(int i = 0; i < num_elements; ++i)
    {
        for(int j = i + 1; j < num_elements; ++j)
        {
            for(int k = j + 1; k < num_elements; ++k)
            {
                if(buffer[i] + buffer[j] + buffer[k] == 2020)
                {
                    return (buffer[i] * buffer[j] * buffer[k]);
                }
            }
        }
    }

    return -1;
}

int main()
{
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
}
