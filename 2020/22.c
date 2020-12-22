
#include <assert.h>
#include <stdio.h>

#define MAX_CARDS 100

void get_input(char *input_filename,
               int *p1_size, int p1[MAX_CARDS],
               int *p2_size, int p2[MAX_CARDS])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    // get player 1's cards
    fscanf(fp, "Player %d:\n", p1_size);
    *p1_size = -1;

    do {
        assert(*p1_size < MAX_CARDS);
    } while(fscanf(fp, "%d\n", p1 + (++(*p1_size))));

    // get player 2's cards
    fscanf(fp, "Player %d:\n", p2_size);
    *p2_size = -1;

    do {
        assert(*p2_size < MAX_CARDS);
    } while(fscanf(fp, "%d\n", p2 + (++(*p2_size))) != EOF);

    fclose(fp);
}

void gain_card(int *size, int deck[MAX_CARDS], int new_card)
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

void lose_card(int *size, int deck[MAX_CARDS])
{
    for(int i = 0; i + 1 < *size; ++i)
    {
        deck[i] = deck[i + 1];
    }
    *size -= 1;
}

int winning_score(int size, int deck[MAX_CARDS])
{
    int score = 0;

    for(int i = 0; i < size; ++i)
    {
        score += deck[i] * (size - i);
    }

    return score;
}

int part1(char *input_filename)
{
    int p1_size;
    int p1[MAX_CARDS];

    int p2_size;
    int p2[MAX_CARDS];

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

    if(p1_size != 0)
    {
        return winning_score(p1_size, p1);
    }
    else
    {
        return winning_score(p2_size, p2);
    }

    assert(0);
}

int part2(char *input_filename)
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
