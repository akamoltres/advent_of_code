
#include <assert.h>
#include <stdio.h>

#define MOD_VALUE 20201227

typedef unsigned long ul;

void get_input(char *input_filename, ul *card_pubk, ul *door_pubk)
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    assert(fscanf(fp, "%lu", card_pubk) != EOF);
    assert(fscanf(fp, "%lu", door_pubk) != EOF);

    fclose(fp);
}

int part1(char *input_filename)
{
    ul card_pubk;
    ul door_pubk;

    get_input(input_filename, &card_pubk, &door_pubk);

    ul card_loop = 0;
    ul subject_number = 7;
    ul value = 1;
    while(value != card_pubk)
    {
        value *= subject_number;
        value %= MOD_VALUE;
        card_loop += 1;
    }

    subject_number = door_pubk;
    value = 1;
    for(ul i = 0; i < card_loop; ++i)
    {
        value *= subject_number;
        value %= MOD_VALUE;
    }

    return value;
}

int part2(char *input_filename)
{
    return -1;
}

int main(int argc, char *argv[])
{
    assert(argc == 2);

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
