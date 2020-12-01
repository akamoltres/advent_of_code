
#include "intcode.h"

#include <stdio.h>

int main()
{
    const int bufsize = 100;
    int buffer[bufsize];

    const int input_bufsize = 1;
    int input_buffer[input_bufsize];

    input_buffer[0] = 12;
    int program_length = read_intcode(bufsize, buffer, "5_test0.txt");
    (void) run_intcode(program_length, buffer, input_bufsize, input_buffer);

    program_length = read_intcode(bufsize, buffer, "5_test1.txt");
    (void) run_intcode(program_length, buffer, 0, NULL);
    printf("%d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);

    program_length = read_intcode(bufsize, buffer, "5_test2.txt");
    (void) run_intcode(program_length, buffer, 0, NULL);
    printf("%d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);

    return 0;
}
