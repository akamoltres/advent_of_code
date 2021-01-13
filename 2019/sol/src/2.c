
#include "intcode.h"

#include <stdio.h>
#include <string.h>

// Returns -1 if failed
int solve_2019_2_1(char *input_filename)
{
    const int bufsize = 200;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, input_filename);
    if(program_length < 0)
    {
        return program_length;
    }

    buffer[1] = 12;
    buffer[2] = 2;

    (void) run_intcode(program_length, bufsize, buffer, 0, NULL, 0);

    return buffer[0];
}

// Returns -1 if failed
int solve_2019_2_2(char *input_filename)
{
    const int bufsize = 200;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, input_filename);
    if(program_length < 0)
    {
        return program_length;
    }

    const int desired_output = 19690720;

    for(int noun = 0; noun < 100; ++noun)
    {
        for(int verb = 0; verb < 100; ++verb)
        {
            int test_program[bufsize];
            memcpy(test_program, buffer, sizeof(int) * bufsize);
            test_program[1] = noun;
            test_program[2] = verb;
            (void) run_intcode(program_length, bufsize, test_program, 0, NULL, 0);
            if(test_program[0] == desired_output)
            {
                return noun * 100 + verb;
            }
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
