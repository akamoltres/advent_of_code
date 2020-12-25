
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

int d_col(int row, char dir)
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

int count_black_tiles(char tiles[GRID_DIM][GRID_DIM])
{
    int count = 0;

    for(int i = 0; i < GRID_DIM; ++i)
    {
        for(int j = 0; j < GRID_DIM; ++j)
        {
            count += tiles[i][j];
        }
    }

    return count;
}

void flip_tiles(int num_flips, char flips[MAX_FLIPS][MAX_STEPS], char tiles[GRID_DIM][GRID_DIM])
{
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
                    c += d_col(r, flips[flip][step + 1]);
                    r += 1;
                    step += 1;
                    break;
                case 'n':
                    c += d_col(r, flips[flip][step + 1]);
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
}

int part1(char *input_filename)
{
    char flips[MAX_FLIPS][MAX_STEPS];
    memset(flips, 0, MAX_FLIPS * MAX_STEPS * sizeof(char));
    char tiles[GRID_DIM][GRID_DIM];
    memset(tiles, 0, GRID_DIM * GRID_DIM * sizeof(char));

    int num_flips = get_input(input_filename, flips);

    flip_tiles(num_flips, flips, tiles);

    return count_black_tiles(tiles);
}

void game_of_life(char tiles[GRID_DIM][GRID_DIM])
{
    char new_tiles[GRID_DIM][GRID_DIM];
    memset(new_tiles, 0, GRID_DIM * GRID_DIM * sizeof(char));

    for(int r = 1; r + 1 < GRID_DIM; ++r)
    {
        for(int c = 1; c + 1 < GRID_DIM; ++c)
        {
            int count = 0;
            count += tiles[r][c - 1];
            count += tiles[r][c + 1];
            count += tiles[r - 1][c + d_col(r, 'e')];
            count += tiles[r - 1][c + d_col(r, 'w')];
            count += tiles[r + 1][c + d_col(r, 'e')];
            count += tiles[r + 1][c + d_col(r, 'w')];

            if(tiles[r][c] == 0 && count == 2)
            {
                new_tiles[r][c] = 1;
                assert(r != 1 && c != 1 && r != GRID_DIM - 2 && c != GRID_DIM - 2);
            }
            else if(tiles[r][c] == 1 && (count == 0 || count > 2))
            {
                new_tiles[r][c] = 0;
            }
            else
            {
                new_tiles[r][c] = tiles[r][c];
            }
        }
    }

    memcpy(tiles, new_tiles, GRID_DIM * GRID_DIM * sizeof(char));
}

int part2(char *input_filename)
{
    char flips[MAX_FLIPS][MAX_STEPS];
    memset(flips, 0, MAX_FLIPS * MAX_STEPS * sizeof(char));
    char tiles[GRID_DIM][GRID_DIM];
    memset(tiles, 0, GRID_DIM * GRID_DIM * sizeof(char));

    int num_flips = get_input(input_filename, flips);

    flip_tiles(num_flips, flips, tiles);

    for(int i = 0; i < 100; ++i)
    {
        game_of_life(tiles);
    }

    return count_black_tiles(tiles);
}

int main(int argc, char *argv[])
{
    assert(argc == 2);

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
