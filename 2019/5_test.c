
#include "intcode.h"

#include <stdio.h>

void test_program(char *filename, int input_bufsize, int *input_buffer)
{
    const int bufsize = 100;
    int buffer[bufsize];

    int program_length = read_intcode(bufsize, buffer, filename);
    if(run_intcode(program_length, bufsize, buffer, input_bufsize, input_buffer) == -1)
    {
        printf("%s failed\n", filename);
    }

    print_program(program_length, buffer);
}

int main()
{
    const int input_bufsize = 1;
    int input_buffer[input_bufsize];

    printf("---Part 1---\n");

    input_buffer[0] = 12;
    test_program("5_test0.txt", input_bufsize, input_buffer);

    test_program("5_test1.txt", 0, NULL);

    test_program("5_test2.txt", 0, NULL);

    printf("---Part 2---\n");

    input_buffer[0] = 7;
    test_program("5_test3.txt", input_bufsize, input_buffer);

    input_buffer[0] = 8;
    test_program("5_test3.txt", input_bufsize, input_buffer);

    input_buffer[0] = 7;
    test_program("5_test4.txt", input_bufsize, input_buffer);

    input_buffer[0] = 8;
    test_program("5_test4.txt", input_bufsize, input_buffer);

    input_buffer[0] = 7;
    test_program("5_test5.txt", input_bufsize, input_buffer);

    input_buffer[0] = 8;
    test_program("5_test5.txt", input_bufsize, input_buffer);

    input_buffer[0] = 7;
    test_program("5_test6.txt", input_bufsize, input_buffer);

    input_buffer[0] = 8;
    test_program("5_test6.txt", input_bufsize, input_buffer);

    input_buffer[0] = 0;
    test_program("5_test7.txt", input_bufsize, input_buffer);

    input_buffer[0] = 1;
    test_program("5_test7.txt", input_bufsize, input_buffer);

    input_buffer[0] = 0;
    test_program("5_test8.txt", input_bufsize, input_buffer);

    input_buffer[0] = 1;
    test_program("5_test8.txt", input_bufsize, input_buffer);

    input_buffer[0] = 7;
    test_program("5_test9.txt", input_bufsize, input_buffer);

    input_buffer[0] = 8;
    test_program("5_test9.txt", input_bufsize, input_buffer);

    input_buffer[0] = 9;
    test_program("5_test9.txt", input_bufsize, input_buffer);

    return 0;
}
