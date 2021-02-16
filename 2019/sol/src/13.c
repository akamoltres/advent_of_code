
#include "intcode.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define GRID_HEIGHT 21
#define GRID_WIDTH 36

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

    Tile_t grid[GRID_HEIGHT][GRID_WIDTH];
    memset(grid, 0, GRID_HEIGHT * GRID_WIDTH * sizeof(Tile_t));

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

        assert(0 <= x && x < GRID_WIDTH && 0 <= y && y < GRID_HEIGHT);

        retval = run_intcode(&program, 0, NULL);
        assert(0 <= retval.retval && retval.retval < NUM_TILE_TYPES);
        block_tile_count -= (grid[y][x] == BLOCK);
        grid[y][x] = retval.retval;
        block_tile_count += (grid[y][x] == BLOCK);
    }

    assert(0);
}

long solve_2019_13_2(char *input_filename)
{
    Intcode_t program;
    memset(program.program, 0, INTCODE_BUFFER_SIZE * sizeof(long)); // TODO go away
    int program_length = read_intcode(&program, input_filename);
    assert(program_length > 0);

    Tile_t grid[GRID_HEIGHT][GRID_WIDTH];
    memset(grid, 0, GRID_HEIGHT * GRID_WIDTH * sizeof(Tile_t));

    // play for free
    // program.program[0] = 2;

    // run the program
    IntcodeReturn_t retval;
    long score = -1;
    while (1)
    {
        retval = run_intcode(&program, 0, NULL);

        if (retval.halt)
        {
            for (int i = 0; i < GRID_HEIGHT; ++i)
            {
                for (int j = 0; j < GRID_WIDTH; ++j)
                {
                    switch (grid[i][j])
                    {
                        case EMPTY:
                            printf(" ");
                            break;
                        case WALL:
                            printf("X");
                            break;
                        case BLOCK:
                            printf("#");
                            break;
                        case HPADDLE:
                            printf("_");
                            break;
                        case BALL:
                            printf("o");
                            break;
                        default:
                            assert(0);
                    }
                }
                printf("\n");
            }
            assert(0);
        }

        int x = retval.retval;

        retval = run_intcode(&program, 0, NULL);
        int y = retval.retval;

        // score display
        if (x == -1 && y == 0)
        {
            retval = run_intcode(&program, 0, NULL);
            score = retval.retval;
            (void) score;
        }
        
        // draw tile
        else
        {
            assert(0 <= x && x < GRID_WIDTH && 0 <= y && y < GRID_HEIGHT);
            retval = run_intcode(&program, 0, NULL);
            assert(0 <= retval.retval && retval.retval < NUM_TILE_TYPES);
            grid[y][x] = retval.retval;
        }
    }

    assert(0);
}
