
#include "intcode.h"

#include <stdio.h>
#include <string.h>

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
