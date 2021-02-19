
#include "intcode.h"

#include <assert.h>
#include <stdio.h>

#define MAP_SIDELEN 50

typedef enum Direction
{
    NORTH = 1,
    SOUTH = 2,
    WEST = 3,
    EAST = 4,
} Direction_t;

typedef enum Square
{
    WALL = 0,
    EMPTY = 1,
    OX_SYSTEM = 2,
    UNKNOWN = 3,
} Square_t;

static int dR(int row, Direction_t dir)
{
    switch (dir)
    {
        case NORTH:
            assert(row > 0);
            return -1;
        case SOUTH:
            assert(row + 1 < MAP_SIDELEN);
            return 1;
        case WEST: // fall through
        case EAST:
            return 0;
        default:
            assert(0);
    }
}

static int dC(int col, Direction_t dir)
{
    switch (dir)
    {
        case WEST:
            assert(col > 0);
            return -1;
        case EAST:
            assert(col + 1 < MAP_SIDELEN);
            return 1;
        case NORTH: // fall through
        case SOUTH:
            return 0;
        default:
            assert(0);
    }
}

static Direction_t opposite_direction(Direction_t dir)
{
    switch (dir)
    {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case WEST:
            return EAST;
        case EAST:
            return WEST;
        default:
            assert(0);
    }
}

// map the area
static void search(Intcode_t *program, Square_t map[MAP_SIDELEN][MAP_SIDELEN], int row, int col,
                   int *ox_r, int *ox_c)
{
    for (int d = 1; d <= 4; ++d)
    {
        // only try new squares
        if (map[row + dR(row, d)][col + dC(col, d)] == UNKNOWN)
        {
            // head that direction, see what happens
            long input_buffer[1];
            input_buffer[0] = d;
            IntcodeReturn_t retval = run_intcode(program, 1, input_buffer);

            map[row + dR(row, d)][col + dC(col, d)] = retval.retval;

            switch (retval.retval)
            {
                case WALL:
                    break;
                case OX_SYSTEM: // fall through
                    *ox_r = row + dR(row, d);
                    *ox_c = col + dC(col, d);
                case EMPTY:
                    search(program, map, row + dR(row, d), col + dC(col, d), ox_r, ox_c);
                    input_buffer[0] = opposite_direction(d);
                    (void)run_intcode(program, 1, input_buffer);
                    break;
                default:
                    assert(0);
            }
        }
    }
}

// find the shortest distance to all squares from the start point
static void shortest_dists(Square_t map[MAP_SIDELEN][MAP_SIDELEN],
                           int dists[MAP_SIDELEN][MAP_SIDELEN], int row, int col)
{
    for (int d = 1; d <= 4; ++d)
    {
        if (map[row + dR(row, d)][col + dC(col, d)] != WALL &&
            dists[row][col] + 1 < dists[row + dR(row, d)][col + dC(col, d)])
        {
            dists[row + dR(row, d)][col + dC(col, d)] = dists[row][col] + 1;
            shortest_dists(map, dists, row + dR(row, d), col + dC(row, d));
        }
    }
}

long solve_2019_15_1(char *input_filename)
{
    Intcode_t program;
    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    int ox_row = -1;
    int ox_col = -1;
    Square_t map[MAP_SIDELEN][MAP_SIDELEN];
    for (int r = 0; r < MAP_SIDELEN; ++r)
    {
        for (int c = 0; c < MAP_SIDELEN; ++c)
        {
            map[r][c] = UNKNOWN;
        }
    }

    // map the area
    search(&program, map, MAP_SIDELEN / 2, MAP_SIDELEN / 2, &ox_row, &ox_col);
    assert(ox_row != -1 && ox_col != -1);

    // setup a structure to store distances from the start point
    int dists[MAP_SIDELEN][MAP_SIDELEN];
    for (int r = 0; r < MAP_SIDELEN; ++r)
    {
        for (int c = 0; c < MAP_SIDELEN; ++c)
        {
            dists[r][c] = MAP_SIDELEN * MAP_SIDELEN;
            if (r == MAP_SIDELEN / 2 && c == MAP_SIDELEN / 2)
            {
                dists[r][c] = 0;
            }
        }
    }

    // find shortest distance to each square from the start point
    shortest_dists(map, dists, MAP_SIDELEN / 2, MAP_SIDELEN / 2);

    return dists[ox_row][ox_col];
}
