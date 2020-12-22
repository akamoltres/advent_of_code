
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARDS 51
#define MAX_ROUNDS 1000000

void get_input(char *input_filename,
               int *p1_size, char p1[MAX_CARDS],
               int *p2_size, char p2[MAX_CARDS])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    // get player 1's cards
    (void) fscanf(fp, "Player %d:\n", p1_size);
    *p1_size = -1;

    do {
        assert(*p1_size < MAX_CARDS);
    } while(fscanf(fp, "%d\n", (int *) (p1 + (++(*p1_size)))));

    // get player 2's cards
    (void) fscanf(fp, "Player %d:\n", p2_size);
    *p2_size = -1;

    do {
        assert(*p2_size < MAX_CARDS);
    } while(fscanf(fp, "%d\n", (int *) (p2 + (++(*p2_size)))) != EOF);

    fclose(fp);
}

void gain_card(int *size, char deck[MAX_CARDS], char new_card)
{
    int winner_card = deck[0];
    for(int i = 0; i + 1 < *size; ++i)
    {
        deck[i] = deck[i + 1];
    }
    deck[*size - 1] = winner_card;
    deck[*size] = new_card;
    *size += 1;
}

void lose_card(int *size, char deck[MAX_CARDS])
{
    for(int i = 0; i + 1 < *size; ++i)
    {
        deck[i] = deck[i + 1];
    }
    *size -= 1;
}

int winning_score(int size, char deck[MAX_CARDS])
{
    int score = 0;

    for(int i = 0; i < size; ++i)
    {
        score += ((int) deck[i]) * (size - i);
    }

    return score;
}

int part1(char *input_filename)
{
    int p1_size;
    char p1[MAX_CARDS];

    int p2_size;
    char p2[MAX_CARDS];

    get_input(input_filename, &p1_size, p1, &p2_size, p2);
    assert(p1_size + p2_size < MAX_CARDS);

    while(p1_size > 0 && p2_size > 0)
    {
        if(p1[0] > p2[0])
        {
            gain_card(&p1_size, p1, p2[0]);
            lose_card(&p2_size, p2);
        }
        else
        {
            gain_card(&p2_size, p2, p1[0]);
            lose_card(&p1_size, p1);
        }
    }

    if(p2_size == 0)
    {
        return winning_score(p1_size, p1);
    }
    if(p1_size == 0)
    {
        return winning_score(p2_size, p2);
    }

    assert(0);
}

int play_recursive_combat(int p1_size, char p1[MAX_CARDS], int p2_size, char p2[MAX_CARDS])
{
    int round_history_1[MAX_CARDS * MAX_CARDS * MAX_CARDS] = {0};
    int round_history_2[MAX_CARDS * MAX_CARDS * MAX_CARDS] = {0};
    for(int i = 0; i < MAX_CARDS * MAX_CARDS * MAX_CARDS; ++i)
    {
        round_history_1[i] = round_history_2[i] = -1;
    }

    int round = 0;
    while(p1_size > 0 && p2_size > 0)
    {
        // check if this configuration has showed up in previous rounds
        if(round_history_1[winning_score(p1_size, p1)] == round_history_2[winning_score(p2_size, p2)] &&
           round_history_1[winning_score(p1_size, p1)] != -1)
        {
            return 1;
        }

        // add this configuration to the history
        round_history_1[winning_score(p1_size, p1)] = round;
        round_history_2[winning_score(p2_size, p2)] = round;

        // who won the round?
        int winner = -1;
        if(p1[0] < p1_size && p2[0] < p2_size)
        {
            char new_p1[MAX_CARDS];
            memcpy(new_p1, p1 + 1, p1[0] * sizeof(char));
            char new_p2[MAX_CARDS];
            memcpy(new_p2, p2 + 1, p2[0] * sizeof(char));

            winner = play_recursive_combat(p1[0], new_p1, p2[0], new_p2);
        }
        else if(p1[0] > p2[0])
        {
            winner = 1;
        }
        else
        {
            winner = 2;
        }
        assert(winner != -1);

        // update the decks
        if(winner == 1)
        {
            gain_card(&p1_size, p1, p2[0]);
            lose_card(&p2_size, p2);
        }
        else
        {
            gain_card(&p2_size, p2, p1[0]);
            lose_card(&p1_size, p1);
        }

    }

    if(p2_size == 0)
    {
        return 1;
    }
    if(p1_size == 0)
    {
        return 2;
    }

    assert(0);
}

int part2(char *input_filename)
{
    int p1_size;
    char p1[MAX_CARDS];

    int p2_size;
    char p2[MAX_CARDS];

    get_input(input_filename, &p1_size, p1, &p2_size, p2);
    assert(p1_size + p2_size < MAX_CARDS);

    int winner = play_recursive_combat(p1_size, p1, p2_size, p2);

    if(winner == 1)
    {
        return winning_score(p1_size + p2_size, p1);
    }
    if(winner == 2)
    {
        return winning_score(p1_size + p2_size, p2);
    }

    assert(0);
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
