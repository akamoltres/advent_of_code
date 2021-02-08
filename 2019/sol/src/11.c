
#include "intcode.h"
#include "util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HULL_DIM 200

typedef enum Color
{
    BLACK = 0,
    WHITE = 1,
} Color_t;

long solve_2019_11_1(char *input_filename)
{
    Intcode_t program;
    memset(program.program, 0, INTCODE_BUFFER_SIZE * sizeof(long)); // TODO go away
    assert(read_intcode(&program, input_filename) > 0);

    // black is 0, white is 1 (all panels start black)
    Color_t panel_colors[MAX_HULL_DIM][MAX_HULL_DIM];
    memset(panel_colors, BLACK, MAX_HULL_DIM * MAX_HULL_DIM * sizeof(Color_t));
    char panel_visited[MAX_HULL_DIM][MAX_HULL_DIM];
    memset(panel_visited, 0, MAX_HULL_DIM * MAX_HULL_DIM * sizeof(char));
    int visited_count = 0;

    // initial state
    // dir: 0 is up, 1 is right, 2 is down, 3 is left
    int row = MAX_HULL_DIM / 2;
    int col = MAX_HULL_DIM / 2;
    int dir = 0;

    // intcode runtime variables
    const int input_bufsize = 1;
    long input_buffer[input_bufsize];
    IntcodeReturn_t retval;

    // run the robot
    while(1)
    {
        // get the updated color for the current panel
        input_buffer[0] = panel_colors[row][col];
        retval = run_intcode(&program, input_bufsize, input_buffer);

        // update the color for the panel
        assert(retval.retval == BLACK || retval.retval == WHITE);
        panel_colors[row][col] = (Color_t) retval.retval;

        // update the visited panels
        if(!panel_visited[row][col])
        {
            visited_count += 1;
            panel_visited[row][col] = 1;
        }

        // get the new direction of the robot
        retval = run_intcode(&program, 0, NULL);

        // end condition
        if(retval.halt)
        {
            break;
        }

        // update the direction
        assert(retval.retval == 0 || retval.retval == 1);
        dir += (retval.retval == 1 ? 1 : -1);
        dir = (dir + 4) % 4;

        // update the robot location
        switch(dir)
        {
            case 0:
                row -= 1;
                break;
            case 1:
                col += 1;
                break;
            case 2:
                row += 1;
                break;
            case 3:
                col -= 1;
                break;
            default:
                assert(0);
        }
        assert(0 <= row && row < MAX_HULL_DIM);
        assert(0 <= col && col < MAX_HULL_DIM);
    }

    return visited_count;
}

char *solve_2019_11_2(char *input_filename)
{
    Intcode_t program;
    memset(program.program, 0, INTCODE_BUFFER_SIZE * sizeof(long)); // TODO go away
    assert(read_intcode(&program, input_filename) > 0);

    // black is 0, white is 1 (all panels start black)
    Color_t panel_colors[MAX_HULL_DIM][MAX_HULL_DIM];
    memset(panel_colors, BLACK, MAX_HULL_DIM * MAX_HULL_DIM * sizeof(Color_t));

    // initial state
    // dir: 0 is up, 1 is right, 2 is down, 3 is left
    // robot starts on a white panel this time
    int row = MAX_HULL_DIM / 2;
    int col = MAX_HULL_DIM / 2;
    int dir = 0;
    panel_colors[row][col] = WHITE;

    // track the robot's fence
    int row_boundaries[2] = {row, row};
    int col_boundaries[2] = {col, col};

    // intcode runtime variables
    const int input_bufsize = 1;
    long input_buffer[input_bufsize];
    IntcodeReturn_t retval;

    // run the robot
    while(1)
    {
        // update the fence
        row_boundaries[0] = min(row_boundaries[0], row);
        row_boundaries[1] = max(row_boundaries[1], row);
        col_boundaries[0] = min(col_boundaries[0], col);
        col_boundaries[1] = max(col_boundaries[1], col);

        // get the updated color for the current panel
        input_buffer[0] = panel_colors[row][col];
        retval = run_intcode(&program, input_bufsize, input_buffer);

        // update the color for the panel
        assert(retval.retval == BLACK || retval.retval == WHITE);
        panel_colors[row][col] = (Color_t) retval.retval;

        // get the new direction of the robot
        retval = run_intcode(&program, 0, NULL);

        // end condition
        if(retval.halt)
        {
            break;
        }

        // update the direction
        assert(retval.retval == 0 || retval.retval == 1);
        dir += (retval.retval == 1 ? 1 : -1);
        dir = (dir + 4) % 4;

        // update the robot location
        switch(dir)
        {
            case 0:
                row -= 1;
                break;
            case 1:
                col += 1;
                break;
            case 2:
                row += 1;
                break;
            case 3:
                col -= 1;
                break;
            default:
                assert(0);
        }
        assert(0 <= row && row < MAX_HULL_DIM);
        assert(0 <= col && col < MAX_HULL_DIM);
    }

    // assemble the result
    int num_rows = row_boundaries[1] - row_boundaries[0] + 1;
    int num_cols = col_boundaries[1] - col_boundaries[0] + 1;
    char *reg_id = (char *) malloc((num_rows * (num_cols + 1) + 1) * sizeof(char));
    memset(reg_id, 0, (num_rows * (num_cols + 1) + 1) * sizeof(char));
    int idx = 0;
    for(int r = row_boundaries[0]; r <= row_boundaries[1]; ++r)
    {
        for(int c = col_boundaries[0]; c <= col_boundaries[1]; ++c)
        {
            reg_id[idx++] = (panel_colors[r][c] == BLACK ? '.' : '#');
        }
        reg_id[idx++] = '\n';
    }

    return reg_id;
}
