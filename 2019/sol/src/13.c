
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
    PADDLE = 3,
    BALL = 4,
    NUM_TILE_TYPES = 5,
} Tile_t;

long solve_2019_13_1(char *input_filename)
{
    Intcode_t program;
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

/*
static void print_grid(Tile_t grid[GRID_HEIGHT][GRID_WIDTH])
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
                case PADDLE:
                    printf("^");
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
}
*/

static char generate_command(Tile_t grid[GRID_HEIGHT][GRID_WIDTH])
{
    static int prev_ball_c = -1;
    static int prev_paddle_c = -1;

    // locate the ball and paddle in this cycle
    int ball_r, ball_c, paddle_c;
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            if (grid[i][j] == BALL)
            {
                ball_r = i;
                ball_c = j;
            }
            else if (grid[i][j] == PADDLE)
            {
                paddle_c = j;
            }
        }
    }

    // initialization case
    if (prev_paddle_c == -1)
    {
        prev_ball_c = ball_c;
        prev_paddle_c = paddle_c;
        return 'c';
    }

    char command = 'c';

    // initially get things lined up
    if (paddle_c != ball_c)
    {
        if (paddle_c > ball_c)
        {
            command = 'l';
        }
        else if (paddle_c < ball_c)
        {
            command = 'r';
        }
    }
    // moving left
    else if (ball_c < prev_ball_c)
    {
        if (grid[ball_r][ball_c - 1] == WALL || grid[ball_r][ball_c - 1] == BLOCK)
        {
            command = 'r';
        }
        else
        {
            command = 'l';
        }
    }
    // moving right
    else if (ball_c > prev_ball_c)
    {
        if (grid[ball_r][ball_c + 1] == WALL || grid[ball_r][ball_c + 1] == BLOCK)
        {
            command = 'l';
        }
        else
        {
            command = 'r';
        }
    }
    // ball didn't move??
    else
    {
        assert(0);
    }

    // update for the next cycle
    prev_ball_c = ball_c;
    prev_paddle_c = paddle_c;

    return command;
}

long solve_2019_13_2(char *input_filename)
{
    Intcode_t program;
    int program_length = read_intcode(&program, input_filename);
    program.interactive_mode = 1;
    assert(program_length > 0);

    Tile_t grid[GRID_HEIGHT][GRID_WIDTH];
    memset(grid, 0, GRID_HEIGHT * GRID_WIDTH * sizeof(Tile_t));

    // play for free
    program.program[0] = 2;

    // run the program
    IntcodeReturn_t retval = run_intcode(&program, 0, NULL);
    long score;
    while (1)
    {
        if (retval.need_input)
        {
            // print_grid(grid);
            long input[1];
            char command = generate_command(grid);
            /* HANDLE USER INPUT */
            /*
            printf("%c\n", command);
            char c[3];
            assert(fgets(c, 3, stdin) != NULL);
            */
            /* END HANDLE USER INPUT */
            switch (command)
            {
                case 'r':
                    input[0] = 1;
                    break;
                case 'l':
                    input[0] = -1;
                    break;
                case 'c':
                    input[0] = 0;
                    break;
                default:
                    assert(0);
            }
            retval = run_intcode(&program, 1, input);
        }
        else if (retval.halt)
        {
            return score;
        }
        else
        {
            long x = retval.retval;
            retval = run_intcode(&program, 0, NULL);
            long y = retval.retval;
            retval = run_intcode(&program, 0, NULL);
            long value = retval.retval;

            if (x == -1 && y == 0)
            {
                score = value;
                // printf("SCORE: %ld\n", value);
            }
            else
            {
                assert(0 <= x && x < GRID_WIDTH && 0 <= y && y < GRID_HEIGHT);
                assert(0 <= value && value < NUM_TILE_TYPES);
                grid[y][x] = value;
            }

            retval = run_intcode(&program, 0, NULL);
        }
    }
}
