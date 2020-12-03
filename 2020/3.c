
#include <stdio.h>
#include <string.h>

#define BUFSIZE 350

typedef struct Pair
{
    int num_rows;
    int num_cols;
} Pair_t;

// Assumes buffer is a square
// Returns pair with input dimensions
// Pair contents are -1 if buffer is too small
Pair_t get_input(const int bufsize, char buffer[BUFSIZE][BUFSIZE])
{
    Pair_t dim;
    dim.num_rows = 0;

    FILE *fp;
    fp = fopen("3.txt", "r");

    while(fscanf(fp, "%s", buffer[dim.num_rows++]) != EOF)
    {
        if(dim.num_rows == bufsize)
        {
            dim.num_rows = -1;
            dim.num_cols = -1;
            return dim;
        }
    }

    dim.num_cols = strlen(buffer[0]);

    fclose(fp);

    return dim;
}

// Returns -1 if failed unexpectedly
int part1(int right_count, int down_count)
{
    const int bufsize = BUFSIZE;
    char buffer[bufsize][bufsize];

    Pair_t dim = get_input(bufsize, buffer);
    if(dim.num_rows == -1)
    {
        return -1;
    }

    int tree_count = 0;
    int cpos = 0;

    for(int rpos = 0; rpos < dim.num_rows; rpos += down_count)
    {
        if(buffer[rpos][cpos] == '#')
        {
            tree_count += 1;
        }
        cpos = (cpos + right_count) % dim.num_cols;
    }

    return tree_count;
}

long part2()
{
    long a = part1(1, 1);
    long b = part1(3, 1);
    long c = part1(5, 1);
    long d = part1(7, 1);
    long e = part1(1, 2);

    if(a == -1 || b == -1 || c == -1 || d == -1 || e == -1)
    {
        return -1;
    }

    return (a * b * c * d * e);
}

int main()
{
    printf("Part 1: %d\n", part1(3, 1));
    printf("Part 2: %ld\n", part2());
}
