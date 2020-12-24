
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PICKED_UP 3

#define NUM_CUPS_1 9

// the part 2 solution would work more efficiently for part 1 as well,
// just leaving this in here to show the brute implementation (for fun)
void play_game(char *order, int num_turns)
{
    int current_cup_idx = 0;

    for(int turn = 1; turn <= num_turns; ++turn)
    {
        char current_cup = order[current_cup_idx];

        // pick up the three cups
        char picked_up[PICKED_UP] = {0};
        for(int i = 1; i <= PICKED_UP; ++i)
        {
            picked_up[i - 1] = order[(current_cup_idx + i) % NUM_CUPS_1];
        }
        for(int i = 0; i < PICKED_UP; ++i)
        {
            for(int j = 0; j < NUM_CUPS_1 - i; ++j)
            {
                if(order[j] == picked_up[i])
                {
                    for(int k = j; k < NUM_CUPS_1 - i; ++k)
                    {
                        order[k] = order[k + 1];
                    }
                    break;
                }
            }
        }

        // select the destination index
        char destination_label = (current_cup == '1' ? '9' : current_cup - 1);
        int destination_idx = -1;
        int present;
        do {
            present = 0;
            for(int i = 0; !present && i < NUM_CUPS_1 - PICKED_UP; ++i)
            {
                if(order[i] == destination_label)
                {
                    present = 1;
                    destination_idx = i;
                }
            }
            if(!present)
            {
                destination_label = (destination_label == '0' ? '9' : destination_label - 1);
            }
        } while(!present);
        assert(destination_idx != -1);

        // put the cups back
        for(int i = NUM_CUPS_1 - 1; i > destination_idx + PICKED_UP; --i)
        {
            order[i] = order[i - PICKED_UP];
        }
        for(int i = 0; i < PICKED_UP; ++i)
        {
            order[destination_idx + i + 1] = picked_up[i];
        }

        // select a new current cup
        current_cup_idx = -1;
        for(int i = 0; current_cup_idx == -1 && i < NUM_CUPS_1; ++i)
        {
            if(order[i] == current_cup)
            {
                current_cup_idx = (i + 1) % NUM_CUPS_1;
            }
        }
    }
}

int part1(char *start_order)
{
    char order[NUM_CUPS_1 + 1] = {0};
    memcpy(order, start_order, NUM_CUPS_1 * sizeof(char));

    // play the game
    play_game(order, 100);

    // get the answer
    int retval = 0;
    for(int i = 0; i < NUM_CUPS_1; ++i)
    {
        if(order[i] == '1')
        {
            for(int j = 0; j < NUM_CUPS_1 - 1; ++j)
            {
                retval = (retval * 10) + (order[(i + j + 1) % NUM_CUPS_1] - '0');
            }
            return retval;
        }
    }

    assert(0);
}

#define NUM_CUPS_2 1000000

typedef struct Cup
{
    int prev_cup;
    int next_cup;
} Cup_t;

void play_game_2(Cup_t *circle, int current_cup, int num_moves)
{
    for(int move = 1; move <= num_moves; ++move)
    {
        // figure out the cups that are picked up
        int picked_up[3];
        int next_cup = circle[current_cup].next_cup;
        for(int i = 0; i < PICKED_UP; ++i)
        {
            picked_up[i] = next_cup;
            next_cup = circle[next_cup].next_cup;
        }

        // identify the destination cup
        int destination_cup = (current_cup == 1 ? NUM_CUPS_2 : current_cup - 1);
        for(int i = 0; i < PICKED_UP * PICKED_UP; ++i)
        {
            if(destination_cup == picked_up[i % PICKED_UP])
            {
                destination_cup = (destination_cup == 1 ? NUM_CUPS_2 : destination_cup - 1);
            }
        }

        // remove the three picked up cups
        int new_next_cup = current_cup;
        for(int i = 0; i < PICKED_UP + 1; ++i)
        {
            new_next_cup = circle[new_next_cup].next_cup;
        }
        circle[current_cup].next_cup = new_next_cup;
        circle[new_next_cup].prev_cup = current_cup;

        // insert the three picked up cups
        new_next_cup = circle[destination_cup].next_cup;
        circle[destination_cup].next_cup = picked_up[0];
        circle[picked_up[0]].prev_cup = destination_cup;
        circle[picked_up[PICKED_UP - 1]].next_cup = new_next_cup;
        circle[new_next_cup].prev_cup = picked_up[PICKED_UP - 1];

        // update the current cup
        current_cup = circle[current_cup].next_cup;
    }
}

unsigned long part2(char *start_order)
{
    Cup_t *circle;
    circle = (Cup_t *) malloc((NUM_CUPS_2 + 1) * sizeof(Cup_t));

    // set up the initial circle
    for(int i = 1; i <= NUM_CUPS_1; ++i)
    {
        int id = (start_order[i - 1] - '0');
        circle[id].prev_cup = (i == 1 ? NUM_CUPS_2 : (start_order[i - 2] - '0'));
        circle[id].next_cup = (i == NUM_CUPS_1 ? NUM_CUPS_1 + 1 : (start_order[i] - '0'));
    }
    for(int i = NUM_CUPS_1 + 1; i <= NUM_CUPS_2; ++i)
    {
        circle[i].prev_cup = i - 1;
        circle[i].next_cup = (i == NUM_CUPS_2 ? (start_order[0] - '0') : i + 1);
    }

    play_game_2(circle, start_order[0] - '0', 10000000);

    // find the solution
    unsigned long first = circle[1].next_cup;
    unsigned long second = circle[first].next_cup;

    free(circle);
    circle = NULL;

    return (first * second);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %lu\n", part2(argv[1]));

    return 0;
}
