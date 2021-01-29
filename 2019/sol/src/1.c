
#include <stdio.h>

unsigned long solve_2019_1_1(char const *input_filename)
{
    unsigned long total_fuel = 0;
    unsigned long module_mass = 0;

    FILE *fp;
    fp = fopen(input_filename, "r");
    while (fscanf(fp, "%lu", &module_mass) == 1)
    {
        total_fuel += module_mass / 3 - 2;
    }
    fclose(fp);

    return total_fuel;
}

long solve_2019_1_2(char const *input_filename)
{
    unsigned long total_fuel = 0;
    unsigned long module_mass = 0;

    FILE *fp;
    fp = fopen(input_filename, "r");
    while (fscanf(fp, "%lu", &module_mass) == 1)
    {
        long fuel_to_add = module_mass / 3 - 2;
        while (fuel_to_add > 0)
        {
            total_fuel += fuel_to_add;
            fuel_to_add = fuel_to_add / 3 - 2;
        }
    }
    fclose(fp);

    return total_fuel;
}

/*int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %lu\n", part1(argv[1]));
    printf("Part 2: %ld\n", part2(argv[1]));

    return 0;
}*/
