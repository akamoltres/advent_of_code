
#include <stdio.h>

typedef unsigned long long ull;

int get_input(char *input_filename, const int bufsize, int *buffer, int *departure)
{
    int bus_length = 0;

    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    fscanf(fp, "%d", departure);

    char c[2];
    fgets(c, 2, fp);    // throw away the newline

    int count = 0;
    while(fgets(c, 2, fp))
    {
        if(c[0] == 'x')
        {
            buffer[bus_length++] = -1;
            fgets(c, 2, fp);    // throw away the comma
        }
        else
        {
            int value = 0;
            int done = 0;
            do {
                if(c[0] == ',')
                {
                    break;
                }
                if(c[0] == '\n' || feof(fp))
                {
                    done = 1;
                    break;
                }

                value = (value * 10) + (c[0] - '0');
            } while(fgets(c, 2, fp));

            buffer[bus_length++] = value;

            if(done)
            {
                break;
            }
        }
        count += 1;
    }

    fclose(fp);

    return bus_length;
}

int part1(char *input_filename)
{
    const int bufsize = 1000;
    int buffer[bufsize];
    int departure;

    int num_buses = get_input(input_filename, bufsize, buffer, &departure);
    if(num_buses == -1)
    {
        return -1;
    }

    int best_bus = -1;
    int best_time = -1;

    for(int i = 0; i < num_buses; ++i)
    {
        if(buffer[i] != -1)
        {
            int time = buffer[i] - (departure % buffer[i]);
            if(best_bus == -1 || time < best_time)
            {
                best_bus = buffer[i];
                best_time = time;
            }
        }
    }

    return (best_bus * best_time);
}

ull gcd(ull a, ull b)
{
    if(a == 0)
    {
        return b;
    }
    return gcd(b % a, a);
}

ull lcm(ull a, ull b)
{
    return ((a / gcd(a, b)) * b);
}

ull part2(char *input_filename)
{
    const int bufsize = 1000;
    int buffer[bufsize];
    int throwaway;

    int num_buses = get_input(input_filename, bufsize, buffer, &throwaway);
    if(num_buses == -1)
    {
        return -1;
    }

    ull departure = 0;
    ull step = 1;
    for(int i = 0; i < num_buses; ++i)
    {
        if(buffer[i] != -1)
        {
            for(; ; departure += step)
            {
                int good = 1;
                for(int j = 0; j <= i; ++j)
                {
                    if(buffer[j] != -1)
                    {
                        if((departure + j) % buffer[j] != 0)
                        {
                            good = 0;
                            break;
                        }
                    }
                }
                if(good)
                {
                    break;
                }
            }
            step *= buffer[i];
        }
    }

    return departure;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %llu\n", part2(argv[1]));

    return 0;
}
