
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NUM_CUPS 9
#define PICKED_UP 3

/*
int get_destination_idx(int current_idx, char order[NUM_CUPS + 1])
{
    char desired_label = (order[current_idx] == '0' ? '9' : order[current_idx] - 1);
}
*/

int part1(char *start_order)
{
    char order[NUM_CUPS + 1] = {0};
    memcpy(order, start_order, NUM_CUPS * sizeof(char));

    int current_cup_idx = 0;

    for(int turn = 1; turn <= 100; ++turn)
    {
        char current_cup = order[current_cup_idx];

        // pick up the three cups
        char picked_up[PICKED_UP] = {0};
        for(int i = 1; i <= PICKED_UP; ++i)
        {
            picked_up[i - 1] = order[(current_cup_idx + i) % NUM_CUPS];
        }
        for(int i = 0; i < PICKED_UP; ++i)
        {
            for(int j = 0; j < NUM_CUPS - i; ++j)
            {
                if(order[j] == picked_up[i])
                {
                    for(int k = j; k < NUM_CUPS - i; ++k)
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
            for(int i = 0; !present && i < NUM_CUPS - PICKED_UP; ++i)
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
        for(int i = NUM_CUPS - 1; i > destination_idx + PICKED_UP; --i)
        {
            order[i] = order[i - PICKED_UP];
        }
        for(int i = 0; i < PICKED_UP; ++i)
        {
            order[destination_idx + i + 1] = picked_up[i];
        }

        // select a new current cup
        current_cup_idx = -1;
        for(int i = 0; current_cup_idx == -1 && i < NUM_CUPS; ++i)
        {
            if(order[i] == current_cup)
            {
                current_cup_idx = (i + 1) % NUM_CUPS;
            }
        }
    }

    // get the answer
    int retval = 0;
    for(int i = 0; i < NUM_CUPS; ++i)
    {
        if(order[i] == '1')
        {
            for(int j = 0; j < NUM_CUPS - 1; ++j)
            {
                retval = (retval * 10) + (order[(i + j + 1) % NUM_CUPS] - '0');
            }
            return retval;
        }
    }

    assert(0);
}

int part2(char *start_order)
{
    return -1;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
