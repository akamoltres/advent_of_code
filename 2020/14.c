
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ADDR_SIZE 36
#define BUFSIZE 1000
#define MEMORY_SIZE 100000

typedef enum CMD
{
    MASK = 0,
    MEM = 1,
} CMD_e;

typedef struct Cmd
{
    CMD_e type;
    uint64_t addr;
    char bitmask[ADDR_SIZE + 1];
    uint64_t value;
} Cmd_t;

int get_input(char *input_filename, Cmd_t *buffer)
{
    int num_cmds = 0;

    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    char input[BUFSIZE];
    while(fscanf(fp, "%s", input) != EOF)
    {
        if(num_cmds == BUFSIZE)
        {
            return -1;
        }

        switch(input[1])
        {
            case 'a':
                buffer[num_cmds].type = MASK;

                fgets(input, 4, fp);    // throw away the " = "

                // get the mask
                fscanf(fp, "%s", buffer[num_cmds].bitmask);

                break;
            case 'e':
                buffer[num_cmds].type = MEM;

                // get the address
                buffer[num_cmds].addr = 0;
                for(int i = 4; i + 1 < strlen(input); ++i)
                {
                    buffer[num_cmds].addr = buffer[num_cmds].addr * 10 + (input[i] - '0');
                }

                fgets(input, 4, fp);    // throw away the " = "

                // get the value
                fscanf(fp, "%lu", &buffer[num_cmds].value);

                break;
            default:
                fclose(fp);
                return -1;
        }
        num_cmds += 1;
    }

    fclose(fp);

    return num_cmds;
}

uint64_t part1(char *input_filename)
{
    Cmd_t buffer[BUFSIZE];

    int num_cmds = get_input(input_filename, buffer);
    if(num_cmds == -1)
    {
        return -1;
    }

    uint64_t memory[MEMORY_SIZE] = {0};
    char bitmask[ADDR_SIZE + 1] = {0};

    for(int i = 0; i < num_cmds; ++i)
    {
        if(buffer[i].type == MASK)
        {
            strcpy(bitmask, buffer[i].bitmask);
        }
        else if(buffer[i].type == MEM)
        {
            if(buffer[i].addr >= MEMORY_SIZE)
            {
                return -1;
            }

            memory[buffer[i].addr] = 0;
            for(int j = 0; j < ADDR_SIZE; ++j)
            {
                uint64_t bit = (buffer[i].value >> j) & 0b1;
                if(bitmask[ADDR_SIZE - j - 1] != 'X')
                {
                    bit = (bitmask[ADDR_SIZE - j - 1] - '0');
                }
                memory[buffer[i].addr] |= (bit << j);
            }
        }
        else
        {
            return -1;
        }
    }

    uint64_t sum = 0;
    for(uint64_t i = 0; i < MEMORY_SIZE; ++i)
    {
        sum += memory[i];
    }

    return sum;
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

    printf("Part 1: %lu\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
