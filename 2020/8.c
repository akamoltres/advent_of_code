
#include <stdio.h>
#include <string.h>

#define MAX_OP_LENGTH 5

typedef struct Command
{
    char op[MAX_OP_LENGTH];
    int arg;
} Command_t;

// Returns number of commands
// Returns -1 if failed unexpectedly
int get_input(const int bufsize, Command_t *buffer)
{
    int num_commands = 0;

    FILE *fp;
    fp = fopen("8.txt", "r");

    if(fp == NULL)
    {
        return -1;
    }

    while(fscanf(fp, "%s", buffer[num_commands].op) != EOF)
    {
        fscanf(fp, "%d", &buffer[num_commands++].arg);
        if(num_commands == bufsize)
        {
            fclose(fp);
            num_commands = -1;
            return num_commands;
        }
    }

    fclose(fp);

    return num_commands;
}

// Returns -1 if failed unexpectedly
int part1()
{
    const int bufsize = 5000;
    Command_t buffer[bufsize];

    int num_commands = get_input(bufsize, buffer);
    if(num_commands == -1)
    {
        return -1;
    }

    int pc = 0;
    int acc = 0;
    int call_count[bufsize];
    memset(call_count, 0, bufsize * sizeof(int));

    while(!call_count[pc])
    {
        call_count[pc] += 1;

        if(!strcmp("nop", buffer[pc].op))
        {
            pc += 1;
        }
        else if(!strcmp("jmp", buffer[pc].op))
        {
            pc += buffer[pc].arg;
        }
        else if(!strcmp("acc", buffer[pc].op))
        {
            acc += buffer[pc].arg;
            pc += 1;
        }
        else
        {
            return -1;
        }
    }

    return acc;
}

// Returns -1 if failed unexpectedly
int part2()
{
    const int bufsize = 5000;
    Command_t buffer[bufsize];

    int num_commands = get_input(bufsize, buffer);
    if(num_commands == -1)
    {
        return -1;
    }

    for(int i = 0; i < num_commands; ++i)
    {
        int pc = 0;
        int acc = 0;

        Command_t program[bufsize];
        memcpy(program, buffer, num_commands * sizeof(Command_t));

        int call_count[bufsize];
        memset(call_count, 0, bufsize * sizeof(int));

        while(!call_count[pc])
        {
            call_count[pc] += 1;

            if(!strcmp("nop", buffer[pc].op))
            {
                if(pc != i)
                {
                    pc += 1;
                }
                else
                {
                    pc += buffer[pc].arg;
                }
            }
            else if(!strcmp("jmp", buffer[pc].op))
            {
                if(pc != i)
                {
                    pc += buffer[pc].arg;
                }
                else
                {
                    pc += 1;
                }
            }
            else if(!strcmp("acc", buffer[pc].op))
            {
                acc += buffer[pc].arg;
                pc += 1;
            }
            else
            {
                return -1;
            }

            if(pc == num_commands)
            {
                return acc;
            }
        }
    }

    return -1;
}

int main()
{
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
}
