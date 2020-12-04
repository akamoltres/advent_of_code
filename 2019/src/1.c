
#include <stdio.h>

unsigned long part1()
{
    unsigned long total_fuel = 0;
    unsigned long module_mass = 0;

    FILE *fp;
    fp = fopen("1.txt", "r");
    while(fscanf(fp, "%lu", &module_mass) == 1)
    {
        total_fuel += module_mass / 3 - 2;
    }
    fclose(fp);
    
    return total_fuel;
}

long part2()
{
    unsigned long total_fuel = 0;
    unsigned long module_mass = 0;

    FILE *fp;
    fp = fopen("1.txt", "r");
    while(fscanf(fp, "%lu", &module_mass) == 1)
    {
        long fuel_to_add = module_mass / 3 - 2;
        while(fuel_to_add > 0)
        {
            total_fuel += fuel_to_add;
            fuel_to_add = fuel_to_add / 3 - 2;
        }
    }
    fclose(fp);
    
    return total_fuel;
}

int main()
{
    printf("Part 1: %lu\n", part1());
    printf("Part 2: %ld\n", part2());
    return 0;
}
