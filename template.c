
#include <assert.h>
#include <stdio.h>

void get_input(char *input_filename)
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    fclose(fp);
}

int part1(char *input_filename)
{
    return -1;
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
