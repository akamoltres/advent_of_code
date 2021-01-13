
#include <stdio.h>
#include <string.h>

#define MAX_OBJECT_LENGTH 100

typedef struct Relation
{
    int orbitee_length;
    char orbitee[MAX_OBJECT_LENGTH];
    int orbiter_length;
    char orbiter[MAX_OBJECT_LENGTH];
    int indirect_orbits;
} Relation_t;

// Returns -1 if failed unexpectedly
int read_orbits(int bufsize, Relation_t *buffer, char *input_filename)
{
    int num_orbits = 0;

    FILE *fp;
    fp = fopen(input_filename, "r");

    if(fp == NULL)
    {
        return -1;
    }

    const int input_bufsize = MAX_OBJECT_LENGTH * MAX_OBJECT_LENGTH + 1;
    char input_buffer[input_bufsize];

    while(fscanf(fp, "%s", input_buffer) != EOF)
    {
        buffer[num_orbits].orbitee_length = 0;
        buffer[num_orbits].orbiter_length = 0;
        buffer[num_orbits].indirect_orbits = -1;
        int paren_found = 0;
        for(int i = 0; i < input_bufsize; ++i)
        {
            if(input_buffer[i] == 0)
            {
                break;
            }

            if(input_buffer[i] == ')')
            {
                paren_found = 1;
                continue;
            }

            if(!paren_found)
            {
                buffer[num_orbits].orbitee[buffer[num_orbits].orbitee_length] = input_buffer[i];
                buffer[num_orbits].orbitee_length += 1;
            }
            else
            {
                buffer[num_orbits].orbiter[buffer[num_orbits].orbiter_length] = input_buffer[i];
                buffer[num_orbits].orbiter_length += 1;
            }
        }

        num_orbits += 1;

        if(num_orbits == bufsize)
        {
            return -1;
        }
    }

    fclose(fp);

    return num_orbits;
}

// Returns -1 if unexpected failure
int count_indirect_orbits(const int bufsize, Relation_t *buffer, int index)
{
    if(!strcmp(buffer[index].orbitee, "COM"))
    {
        buffer[index].indirect_orbits = 0;
        return 0;
    }

    for(int i = 0; i < bufsize; ++i)
    {
        if(i == index)
        {
            continue;
        }

        if(!strcmp(buffer[index].orbitee, buffer[i].orbiter))
        {
            if(buffer[i].indirect_orbits == -1)
            {
                count_indirect_orbits(bufsize, buffer, i);
            }
            buffer[index].indirect_orbits = buffer[i].indirect_orbits + 1;
            return buffer[index].indirect_orbits;
        }
    }

    return -1;
}

// Returns -1 if unexpected failure
int solve_2019_6_1(char *input_filename)
{
    const int bufsize = 5000;
    Relation_t buffer[bufsize];
    memset(buffer, 0, bufsize * sizeof(Relation_t));

    int total_direct_orbits = read_orbits(bufsize, buffer, input_filename);

    if(total_direct_orbits == -1)
    {
        return -1;
    }

    // Compute indirect orbits
    for(int i = 0; i < total_direct_orbits; ++i)
    {
        if(buffer[i].indirect_orbits == -1)
        {
            int indirect_orbits = count_indirect_orbits(bufsize, buffer, i);

            if(indirect_orbits == -1)
            {
                return -1;
            }
        }
    }

    // Count indirect orbits
    int total_indirect_orbits = 0;
    for(int i = 0; i < total_direct_orbits; ++i)
    {
        total_indirect_orbits += buffer[i].indirect_orbits;
    }

    return (total_indirect_orbits + total_direct_orbits);
}

// Returns -1 if unexpected failure
int index_of_orbiter(const int bufsize, Relation_t *buffer, char *target)
{
    for(int i = 0; i < bufsize; ++i)
    {
        if(!strcmp(buffer[i].orbiter, target))
        {
            return i;
        }
    }

    return -1;
}

// Returns -1 if unexpected failure
int build_chain(const int bufsize, Relation_t *buffer, int *chain_buffer)
{
    int chain_length = 1;

    while(strcmp(buffer[chain_buffer[chain_length - 1]].orbitee, "COM"))
    {
        if(chain_length == bufsize)
        {
            return -1;
        }

        chain_buffer[chain_length] = index_of_orbiter(bufsize, buffer, buffer[chain_buffer[chain_length - 1]].orbitee);
        chain_length += 1;

        if(chain_buffer[chain_length - 1] == -1)
        {
            return -1;
        }
    }

    return chain_length;
}

// Returns -1 if unexpected failure
int solve_2019_6_2(char *input_filename)
{
    const int bufsize = 5000;
    Relation_t buffer[bufsize];
    memset(buffer, 0, bufsize * sizeof(Relation_t));

    int total_direct_orbits = read_orbits(bufsize, buffer, input_filename);

    if(total_direct_orbits == -1)
    {
        return -1;
    }

    // Find the starting points
    int index_you = -1;
    int index_san = -1;
    for(int i = 0; i < total_direct_orbits; ++i)
    {
        if(!strcmp(buffer[i].orbiter, "YOU"))
        {
            index_you = i;
        }
        else if(!strcmp(buffer[i].orbiter, "SAN"))
        {
            index_san = i;
        }
    }

    // Build chains to COM for YOU and SAN
    int chain_buffer_you[bufsize];
    chain_buffer_you[0] = index_you;
    int chain_length_you = build_chain(total_direct_orbits, buffer, chain_buffer_you);

    int chain_buffer_san[bufsize];
    chain_buffer_san[0] = index_san;
    int chain_length_san = build_chain(total_direct_orbits, buffer, chain_buffer_san);

    if(chain_length_you == -1 || chain_length_san == -1)
    {
        return -1;
    }

    // Find where the chains stop lining up
    int min_chain_length = (chain_length_you < chain_length_san ? chain_length_you : chain_length_san);
    for(int i = 1; i <= min_chain_length; ++i)
    {
        if(chain_buffer_you[chain_length_you - i] != chain_buffer_san[chain_length_san - i])
        {
            return ((chain_length_you - i) + (chain_length_san - i));
        }
    }

    return -1;
}

/*int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}*/
