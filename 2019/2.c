
#include <stdio.h>
#include <string.h>

// Returns length of intcode program if success
// Returns -1 if file is invalid
// Returns -2 if buffer is not long enough
int read_intcode(const int bufsize, int *buffer, char *filename)
{
    int program_length = 0;

    FILE *fp;
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        return -1;
    }

    int done = 0;
    while(1)
    {
        int value = 0;
        char c[2];
        while(fgets(c, 2, fp))
        {
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
        }

        buffer[program_length++] = value;

        if(done)
        {
            fclose(fp);
            return program_length;
        }

        if(program_length == bufsize)
        {
            return -2;
        }
    }
}

// Returns 0 if program terminated successfully (opcode 99)
// Returns -1 if program terminated unexpectedly
int run_intcode(const int program_length, int *buffer)
{
    int position = 0;

    while(buffer[position] != 99)
    {
        switch(buffer[position])
        {
        case 1: // add
            buffer[buffer[position + 3]] = buffer[buffer[position + 1]] + buffer[buffer[position + 2]];
            break;
        case 2: // multiply
            buffer[buffer[position + 3]] = buffer[buffer[position + 1]] * buffer[buffer[position + 2]];
            break;
        default:
            return -1;
            break;
        }

        position += 4;
    }

    return 0;
}

int part1()
{
    const int bufsize = 200;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, "2.txt");

    buffer[1] = 12;
    buffer[2] = 2;

    (void) run_intcode(program_length, buffer);

    return buffer[0];
}

// Returns -1 if failed
int part2()
{
    const int bufsize = 200;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, "2.txt");

    const int desired_output = 19690720;

    for(int noun = 0; noun < 100; ++noun)
    {
        for(int verb = 0; verb < 100; ++verb)
        {
            int test_program[bufsize];
            memcpy(test_program, buffer, sizeof(int) * bufsize);
            test_program[1] = noun;
            test_program[2] = verb;
            (void) run_intcode(program_length, test_program);
            if(test_program[0] == desired_output)
            {
                return noun * 100 + verb;
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
