
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_STEPS 500
#define MAX_FLIPS 500
#define GRID_DIM 500

int get_input(char *input_filename, char flips[MAX_FLIPS][MAX_STEPS])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    int num_flips = 0;

    while(fscanf(fp, "%s\n", flips[num_flips++]) != EOF)
    {
        assert(num_flips != MAX_FLIPS);
    }

    fclose(fp);

    return num_flips - 1;
}

int d_col(int row, int col, char dir)
{
    if(row % 2 == 0)
    {
        return (dir == 'e' ? 0 : -1);
    }
    else
    {
        return (dir == 'e' ? 1 : 0);
    }

    assert(0);
}

int part1(char *input_filename)
{
    char flips[MAX_FLIPS][MAX_STEPS];
    memset(flips, 0, MAX_FLIPS * MAX_STEPS * sizeof(char));
    char tiles[GRID_DIM][GRID_DIM];
    memset(tiles, 0, GRID_DIM * GRID_DIM * sizeof(char));

    int num_flips = get_input(input_filename, flips);

    const int rtr = GRID_DIM / 2;
    const int rtc = GRID_DIM / 2;

    for(int flip = 0; flip < num_flips; ++flip)
    {
        int r = rtr;
        int c = rtc;

        for(int step = 0; step < strlen(flips[flip]); ++step)
        {
            switch(flips[flip][step])
            {
                case 'e':
                    c += 1;
                    break;
                case 'w':
                    c -= 1;
                    break;
                case 's':
                    c += d_col(r, c, flips[flip][step + 1]);
                    r += 1;
                    step += 1;
                    break;
                case 'n':
                    c += d_col(r, c, flips[flip][step + 1]);
                    r -= 1;
                    step += 1;
                    break;
                default:
                    assert(0);
            }

            assert(0 <= r && r < GRID_DIM);
            assert(0 <= c && c < GRID_DIM);
        }

        tiles[r][c] = 1 - tiles[r][c];
    }

    int black_count = 0;
    for(int i = 0; i < GRID_DIM; ++i)
    {
        for(int j = 0; j < GRID_DIM; ++j)
        {
            black_count += tiles[i][j];
        }
    }

    return black_count;
}

int part2(char *input_filename)
{
    return -1;
}

int main(int argc, char *argv[])
{
    assert(argc == 2);

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
