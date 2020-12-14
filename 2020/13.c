
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
    int departure;

    int num_buses = get_input(input_filename, bufsize, buffer, &departure);
    if(num_buses == -1)
    {
        return -1;
    }

    int max_bus_idx = -1;
    for(int i = 0; i < num_buses; ++i)
    {
        if(buffer[i] != -1)
        {
            if(buffer[max_bus_idx] < buffer[i] || max_bus_idx == -1)
            {
                max_bus_idx = i;
            }
        }
    }

    /*
    int start_idx = -1;
    while(buffer[++start_idx] == -1);
    ull lcm_total = buffer[start_idx];
    for(int i = start_idx + 1; i < num_buses; ++i)
    {
        if(buffer[i] != -1)
        {
            lcm_total = lcm(lcm_total, buffer[i]);
        }
    }
    printf("%llu\n", lcm_total);
    return -1;
    */

    // ull start_ts = 100000000000000 + (buffer[max_bus_idx] - (100000000000000 % buffer[max_bus_idx]));
    // ull start_ts = buffer[max_bus_idx];
    // ull start_ts = 1658064091492211;
    ull start_ts = 1;
    for(int i = 0; i < num_buses; ++i)
    {
        if(buffer[i] != -1)
        {
            start_ts *= buffer[i];
        }
    }
    printf("start: %llu\n", start_ts);

    for(ull timestamp = start_ts; timestamp > buffer[max_bus_idx]; timestamp -= buffer[max_bus_idx])
    {
        int fail = 0;
        for(int i = 0; i < num_buses; ++i)
        {
            if(buffer[i] != -1)
            {
                if(i < max_bus_idx)
                {
                    if(max_bus_idx - i != timestamp % buffer[i])
                    {
                        fail = 1;
                        break;
                    }
                }
                else if(max_bus_idx < i)
                {
                    if(i - max_bus_idx != buffer[i] - (timestamp % buffer[i]))
                    {
                        fail = 1;
                        break;
                    }
                }
            }
        }
        if(!fail)
        {
            printf("%llu\n", timestamp - max_bus_idx);
        }
    }

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
    printf("Part 2: %llu\n", part2(argv[1]));

    return 0;
}
