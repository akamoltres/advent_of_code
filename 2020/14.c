
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

uint64_t memory_idx(uint64_t mem_length, uint64_t *memory, uint64_t value)
{
    for(int i = 0; i < mem_length; ++i)
    {
        if(memory[i] == value)
        {
            return i;
        }
    }

    return -1;
}

uint64_t part2(char *input_filename)
{
    Cmd_t buffer[BUFSIZE];

    int num_cmds = get_input(input_filename, buffer);
    if(num_cmds == -1)
    {
        return -1;
    }

    uint64_t memory_addrs[MEMORY_SIZE];
    uint64_t memory[MEMORY_SIZE];
    uint64_t mem_length = 0;

    char bitmask[ADDR_SIZE + 1] = {0};

    for(int i = 0; i < num_cmds; ++i)
    {
        if(buffer[i].type == MASK)
        {
            strcpy(bitmask, buffer[i].bitmask);
        }
        else if(buffer[i].type == MEM)
        {
            char addr_mask[ADDR_SIZE + 1] = {0};
            uint64_t num_addrs = 1;
            for(int j = 0; j < ADDR_SIZE; ++j)
            {
                if(bitmask[j] == 'X')
                {
                    addr_mask[j] = 'X';
                    num_addrs <<= 1;
                }
                else
                {
                    addr_mask[j] = ((((buffer[i].addr >> (ADDR_SIZE - j - 1)) & 0b1) | (bitmask[j] - '0')) ? '1' : '0');
                }
            }

            for(int j = 0; j < num_addrs; ++j)
            {
                uint64_t floating_bits = j;
                uint64_t addr = 0;

                for(int k = 0; k < ADDR_SIZE; ++k)
                {
                    uint64_t bit;

                    if(addr_mask[k] == 'X')
                    {
                        bit = (floating_bits & 0b1);
                        floating_bits >>= 1;
                    }
                    else
                    {
                        bit = (addr_mask[k] - '0');
                    }

                    addr = (addr << 1) | bit;
                }

                uint64_t index = memory_idx(mem_length, memory_addrs, addr);

                if(index == -1)
                {
                    if(mem_length == MEMORY_SIZE)
                    {
                        return -1;
                    }

                    memory_addrs[mem_length] = addr;
                    memory[mem_length] = buffer[i].value;
                    mem_length += 1;
                }
                else
                {
                    memory[index] = buffer[i].value;
                }
            }
        }
        else
        {
            return -1;
        }
    }

    uint64_t sum = 0;
    for(uint64_t i = 0; i < mem_length; ++i)
    {
        sum += memory[i];
    }

    return sum;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %lu\n", part1(argv[1]));
    printf("Part 2: %lu\n", part2(argv[1]));

    return 0;
}
