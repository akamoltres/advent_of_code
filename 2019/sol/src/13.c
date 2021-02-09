
#include "intcode.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define GRID_SIZE 100

typedef enum Tile
{
    EMPTY = 0,
    WALL = 1,
    BLOCK = 2,
    HPADDLE = 3,
    BALL = 4,
    NUM_TILE_TYPES = 5,
} Tile_t;

long solve_2019_13_1(char *input_filename)
{
    Intcode_t program;
    memset(program.program, 0, INTCODE_BUFFER_SIZE * sizeof(long)); // TODO go away
    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    char grid[GRID_SIZE][GRID_SIZE];
    memset(grid, 0, GRID_SIZE * GRID_SIZE * sizeof(char));

    // run the program, tracking the block tiles
    IntcodeReturn_t retval;
    int block_tile_count = 0;
    while (1)
    {
        retval = run_intcode(&program, 0, NULL);
        if (retval.halt)
        {
            return block_tile_count;
        }
        int x = retval.retval;

        retval = run_intcode(&program, 0, NULL);
        int y = retval.retval;

        assert(0 <= x && x < GRID_SIZE && 0 <= y && y < GRID_SIZE);

        retval = run_intcode(&program, 0, NULL);
        assert(0 <= retval.retval && retval.retval < NUM_TILE_TYPES);
        block_tile_count -= (grid[y][x] == BLOCK);
        grid[y][x] = retval.retval;
        block_tile_count += (grid[y][x] == BLOCK);
    }

    assert(0);
}
