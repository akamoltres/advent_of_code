
#include "io.h"
#include "util.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct StationLocation
{
    int r;
    int c;
    int in_view;
} StationLocation_t;

int count_asteroids_in_view(char map[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR], int rc, int cc, int row, int col)
{
    int count = 0;

    // try all the other asteroids
    for(int r = 0; r < rc; ++r)
    {
        for(int c = 0; c < cc; ++c)
        {
            // not the one with the station, there is an asteroid there
            if(!(r == row && c == col) && map[r][c] == '#')
            {
                int visible = 1;

                // finds all asteroids that line up perfectly between the station
                // and the test asteroid
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

StationLocation_t get_station_location(char map[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR], int rows, int cols)
{
    StationLocation_t retval = {
        .r = 0,
        .c = 0,
        .in_view = 0,
    };

    // count number of visible asteroids from each asteroid
    for(int r = 0; r < rows; ++r)
    {
        for(int c = 0; c < rows; ++c)
        {
            if(map[r][c] == '#')
            {
                int asteroid_count = count_asteroids_in_view(map, rows, cols, r, c);
                if(asteroid_count > retval.in_view)
                {
                    retval.r = r;
                    retval.c = c;
                    retval.in_view = asteroid_count;
                }
            }
        }
    }

    return retval;
}

int solve_2019_10_1(char const *input_filename)
{
    char map[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR];
    int rows = read_str_lines(input_filename, map);
    int cols = strlen(map[0]) - 1;

    return get_station_location(map, rows, cols).in_view;
}

#define MAX_NUM_ASTEROIDS 1000
#define EPSILON 0.000001

typedef struct Asteroid
{
    int r;
    int c;
    int dx;
    int dy;
    double angle;
    int dist;
    int vaporized;
} Asteroid_t;

int solve_2019_10_2(char const *input_filename)
{
    char map[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR];
    int rows = read_str_lines(input_filename, map);
    int cols = strlen(map[0]) - 1;

    // find the station location
    StationLocation_t station = get_station_location(map, rows, cols);

    // make a list of all the asteroids that we need to vaporize
    Asteroid_t asteroids[MAX_NUM_ASTEROIDS];
    int num_asteroids = 0;
    for(int r = 0; r < rows; ++r)
    {
        for(int c = 0; c < cols; ++c)
        {
            if(map[r][c] == '#' && !(r == station.r && c == station.c))
            {
                asteroids[num_asteroids].r = r;
                asteroids[num_asteroids].c = c;
                asteroids[num_asteroids].dx = (station.r - r);
                asteroids[num_asteroids].dy = (station.c - c);
                asteroids[num_asteroids].angle = atan2(asteroids[num_asteroids].dy, asteroids[num_asteroids].dx);
                if(asteroids[num_asteroids].angle < EPSILON)
                {
                    asteroids[num_asteroids].angle += 2 * M_PI;
                }
                asteroids[num_asteroids].dist = abs(r - station.r) + abs(c - station.c);
                asteroids[num_asteroids].vaporized = 0;
                num_asteroids += 1;
                assert(num_asteroids < MAX_NUM_ASTEROIDS);
            }
        }
    }

    // sort all the asteroids by angle (and if equal angle, by distance)
    for(int i = 0; i < num_asteroids; ++i)
    {
        for(int j = 0; j + 1 < num_asteroids; ++j)
        {
            if(fabs(asteroids[j].angle - asteroids[j + 1].angle) < EPSILON)
            {
                if(asteroids[j].dist > asteroids[j + 1].dist)
                {
                    Asteroid_t temp = asteroids[j + 1];
                    asteroids[j + 1] = asteroids[j];
                    asteroids[j] = temp;
                }
            }
            else if(asteroids[j].angle < asteroids[j + 1].angle)
            {
                Asteroid_t temp = asteroids[j + 1];
                asteroids[j + 1] = asteroids[j];
                asteroids[j] = temp;
            }
        }
    }

    // vaporize the asteroids
    int idx = 0;
    int vaporized = 0;
    int retval = 0;
    while(vaporized < 200)
    {
        retval = asteroids[idx].c * 100 + asteroids[idx].r;
        asteroids[idx].vaporized = 1;
        while(asteroids[idx].vaporized == 1 ||
              fabs(asteroids[((idx - 1) + num_asteroids) % num_asteroids].angle - asteroids[idx].angle) < EPSILON)
        {
            idx += 1;
        }
        vaporized += 1;
    }

    return retval;
}
