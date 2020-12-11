
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 2000

typedef struct Pair
{
    int num_rows;
    int num_cols;
} Pair_t;

Pair_t get_input(char *input_filename, char buffer[BUFSIZE][BUFSIZE])
{
    Pair_t dim;
    dim.num_rows = 0;
    dim.num_cols = 0;

    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        dim.num_rows = -1;
        dim.num_cols = -1;
        return dim;
    }

    while(fscanf(fp, "%s", buffer[dim.num_rows++]) != EOF)
    {
        if(dim.num_rows == BUFSIZE)
        {
            dim.num_rows = -1;
            dim.num_cols = -1;
            fclose(fp);
            return dim;
        }
    }

    dim.num_rows -= 1;
    dim.num_cols = strlen(buffer[0]);

    fclose(fp);

    return dim;
}

int count_occupied_adjacent(Pair_t dim, char buffer[BUFSIZE][BUFSIZE], int row, int col)
{
    assert(0 <= row && row < dim.num_rows);
    assert(0 <= col && col < dim.num_cols);
    
    int count = 0;

    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            if(!((i == 0 && j == 0) ||
                 row + i < 0 || dim.num_rows == row + i ||
                 col + j < 0 || dim.num_cols == col + j))
            {
                count += (buffer[row + i][col + j] == '#');
            }
        }
    }

    return count;
}

int buffers_same(Pair_t dim, char buf1[BUFSIZE][BUFSIZE], char buf2[BUFSIZE][BUFSIZE])
{
    for(int i = 0; i < dim.num_rows; ++i)
    {
        for(int j = 0; j < dim.num_rows; ++j)
        {
            if(buf1[i][j] != buf2[i][j])
            {
                return 0;
            }
        }
    }

    return 1;
}

int part1(char *input_filename)
{
    char buffer1[BUFSIZE][BUFSIZE];
    char buffer2[BUFSIZE][BUFSIZE];
    memset(buffer1, 0, BUFSIZE * BUFSIZE * sizeof(char));
    memset(buffer2, 0, BUFSIZE * BUFSIZE * sizeof(char));

    Pair_t dim = get_input(input_filename, buffer2);
    if(dim.num_rows == -1)
    {
        return -1;
    }

    while(!buffers_same(dim, buffer1, buffer2))
    {
        memcpy(buffer1, buffer2, BUFSIZE * BUFSIZE * sizeof(char));

        for(int i = 0; i < dim.num_rows; ++i)
        {
            for(int j = 0; j < dim.num_cols; ++j)
            {
                int occupied = count_occupied_adjacent(dim, buffer1, i, j);
                if(buffer1[i][j] == 'L' && occupied == 0)
                {
                    buffer2[i][j] = '#';
                }
                else if(buffer1[i][j] == '#' && occupied >= 4)
                {
                    buffer2[i][j] = 'L';
                }
            }
        }
    }

    int count = 0;

    for(int i = 0; i < dim.num_rows; ++i)
    {
        for(int j = 0; j < dim.num_cols; ++j)
        {
            count += (buffer1[i][j] == '#');
        }
    }

    return count;
}

int count_occupied_linear(Pair_t dim, char buffer[BUFSIZE][BUFSIZE], int row, int col)
{
    assert(0 <= row && row < dim.num_rows);
    assert(0 <= col && col < dim.num_cols);
    
    int count = 0;

    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            if(i == 0 && j == 0)
            {
                continue;
            }

            int dist = 1;
            while(0 <= row + dist * i && row + dist * i < dim.num_rows &&
                  0 <= col + dist * j && col + dist * j < dim.num_cols)
            {
                if(buffer[row + dist * i][col + dist * j] == '#')
                {
                    count += 1;
                    break;
                }
                if(buffer[row + dist * i][col + dist * j] != '.')
                {
                    break;
                }
                dist += 1;
            }
        }
    }

    return count;
}

int part2(char *input_filename)
{
    char buffer1[BUFSIZE][BUFSIZE];
    char buffer2[BUFSIZE][BUFSIZE];
    memset(buffer1, 0, BUFSIZE * BUFSIZE * sizeof(char));
    memset(buffer2, 0, BUFSIZE * BUFSIZE * sizeof(char));

    Pair_t dim = get_input(input_filename, buffer2);
    if(dim.num_rows == -1)
    {
        return -1;
    }

    while(!buffers_same(dim, buffer1, buffer2))
    {
        memcpy(buffer1, buffer2, BUFSIZE * BUFSIZE * sizeof(char));

        for(int i = 0; i < dim.num_rows; ++i)
        {
            for(int j = 0; j < dim.num_cols; ++j)
            {
                int occupied = count_occupied_linear(dim, buffer1, i, j);
                if(buffer1[i][j] == 'L' && occupied == 0)
                {
                    buffer2[i][j] = '#';
                }
                else if(buffer1[i][j] == '#' && occupied >= 5)
                {
                    buffer2[i][j] = 'L';
                }
            }
        }
    }

    int count = 0;

    for(int i = 0; i < dim.num_rows; ++i)
    {
        for(int j = 0; j < dim.num_cols; ++j)
        {
            count += (buffer1[i][j] == '#');
        }
    }

    return count;
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
