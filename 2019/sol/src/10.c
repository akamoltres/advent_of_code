
#include "io.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>

int count_asteroids_in_view(char map[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR], int rc, int cc, int row, int col)
{
    int count = 0;

    for(int r = 0; r < rc; ++r)
    {
        for(int c = 0; c < cc; ++c)
        {
            if(!(r == row && c == col) && map[r][c] == '#')
            {
                int visible = 1;

                int div = gcd(abs(r - row), abs(c - col));
                int dr = (r - row) / div;
                int dc = (c - col) / div;
                for(int i = 1; i * dr != (r - row) || i * dc != (c - col); ++i)
                {
                    if(map[row + dr * i][col + dc * i] == '#')
                    {
                        visible = 0;
                    }
                }

                count += visible;
            }
        }
    }

    return count;
}

int solve_2019_10_1(char const *input_filename)
{
    char map[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR];
    int rows = read_str_lines(input_filename, map);
    int cols = strlen(map[0]) - 1;

    int most_asteroids = 0;

    for(int r = 0; r < rows; ++r)
    {
        for(int c = 0; c < rows; ++c)
        {
            if(map[r][c] == '#')
            {
                int asteroid_count = count_asteroids_in_view(map, rows, cols, r, c);
                most_asteroids = (asteroid_count > most_asteroids ? asteroid_count : most_asteroids);
            }
        }
    }

    return most_asteroids;
}
