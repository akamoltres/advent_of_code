
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// we'll use the middle of this as the origin
// by adding BUFSIZE/2 to all points
#define BUFSIZE 100

typedef struct CharPair
{
    char a;
    char b;
} CharPair_t;

int read_input(char *input_filename, CharPair_t ***buffer)
{
    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    int row = BUFSIZE / 2;
    int col = BUFSIZE / 2;
    char c[2];
    while(fgets(c, 2, fp) != NULL)
    {
        if(c[0] == '\n')
        {
            row += 1;
            col = BUFSIZE / 2;
            continue;
        }
        assert(row < BUFSIZE);
        assert(col + 1 < BUFSIZE);
        buffer[row][col++][BUFSIZE / 2].a = c[0];
    }

    fclose(fp);

    return 0;
}

void free_buffer(CharPair_t ***buffer)
{
    for(int i = 0; i < BUFSIZE; ++i)
    {
        for(int j = 0; j < BUFSIZE; ++j)
        {
            free(buffer[i][j]);
        }
        free(buffer[i]);
    }
    free(buffer);
}

int part1(char *input_filename)
{
    // heap reaxx only
    CharPair_t ***buffer;
    buffer = (CharPair_t ***) malloc(BUFSIZE * sizeof(CharPair_t **));
    for(int i = 0; i < BUFSIZE; ++i)
    {
        buffer[i] = (CharPair_t **) malloc(BUFSIZE * sizeof(CharPair_t *));
        for(int j = 0; j < BUFSIZE; ++j)
        {
            buffer[i][j] = (CharPair_t *) malloc(BUFSIZE * sizeof(CharPair_t));
            for(int k = 0; k < BUFSIZE; ++k)
            {
                buffer[i][j][k].a = buffer[i][j][k].b = '.';
            }
        }
    }

    // get the initial state
    if(read_input(input_filename, buffer))
    {
        return -1;
    }

    // run through the steps
    for(int s = 0; s < 6; ++s)
    {
        // update all the cubes
        for(int i = 1; i + 1 < BUFSIZE; ++i)
        {
            for(int j = 1; j + 1 < BUFSIZE; ++j)
            {
                for(int k = 1; k + 1 < BUFSIZE; ++k)
                {
                    // count active neighbors
                    int active_neighbors = 0;
                    for(int a = -1; a <= 1; ++a)
                    {
                        for(int b = -1; b <= 1; ++b)
                        {
                            for(int c = -1; c <= 1; ++c)
                            {
                                if(a == 0 && b == 0 && c == 0)
                                {
                                    continue;
                                }
                                active_neighbors += (buffer[i + a][j + b][k + c].a == '#');
                            }
                        }
                    }

                    if(buffer[i][j][k].a == '#')
                    {
                        if(active_neighbors != 2 && active_neighbors != 3)
                        {
                            buffer[i][j][k].b = '.';
                        }
                        else
                        {
                            assert(i != 1 && i != BUFSIZE - 2 &&
                                   j != 1 && j != BUFSIZE - 2 &&
                                   k != 1 && k != BUFSIZE - 2);
                            buffer[i][j][k].b = '#';
                        }
                    }
                    else if(buffer[i][j][k].a == '.')
                    {
                        if(active_neighbors == 3)
                        {
                            assert(i != 1 && i != BUFSIZE - 2 &&
                                   j != 1 && j != BUFSIZE - 2 &&
                                   k != 1 && k != BUFSIZE - 2);
                            buffer[i][j][k].b = '#';
                        }
                        else
                        {
                            buffer[i][j][k].b = '.';
                        }
                    }
                    else
                    {
                        free_buffer(buffer);
                        return -1;
                    }
                }
            }
        }

        // copy the new state into the old state
        for(int i = 0; i < BUFSIZE; ++i)
        {
            for(int j = 0; j < BUFSIZE; ++j)
            {
                for(int k = 0; k < BUFSIZE; ++k)
                {
                    buffer[i][j][k].a = buffer[i][j][k].b;
                }
            }
        }
    }

    // count active cubes
    int active_cubes = 0;
    for(int i = 0; i < BUFSIZE; ++i)
    {
        for(int j = 0; j < BUFSIZE; ++j)
        {
            for(int k = 0; k < BUFSIZE; ++k)
            {
                active_cubes += (buffer[i][j][k].a == '#');
            }
        }
    }

    // clean up
    free_buffer(buffer);

    return active_cubes;
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
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
