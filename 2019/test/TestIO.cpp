
#include "CppUTest/TestHarness.h"
#include "io.h"

#include <string.h>

TEST_GROUP(IO_CSV_LINE)
{
};

TEST(IO_CSV_LINE, CSV_LINE_0)
{
    long buffer[1000];
    int num_inputs = read_csv_line("input/2.txt", buffer, 1000);
    CHECK_EQUAL(165, num_inputs);

    long expected_buffer[165] = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,1,10,19,1,6,19,23,1,10,23,27,2,27,13,31,1,31,6,35,2,6,35,39,1,39,5,43,1,6,43,47,2,6,47,51,1,51,5,55,2,55,9,59,1,6,59,63,1,9,63,67,1,67,10,71,2,9,71,75,1,6,75,79,1,5,79,83,2,83,10,87,1,87,5,91,1,91,9,95,1,6,95,99,2,99,10,103,1,103,5,107,2,107,6,111,1,111,5,115,1,9,115,119,2,119,10,123,1,6,123,127,2,13,127,131,1,131,6,135,1,135,10,139,1,13,139,143,1,143,13,147,1,5,147,151,1,151,2,155,1,155,5,0,99,2,0,14,0};
    for(int i = 0; i < num_inputs; ++i)
    {
        CHECK_EQUAL(expected_buffer[i], buffer[i]);
    }
}

TEST(IO_CSV_LINE, CSV_LINE_1)
{
    long buffer[1000];
    int num_inputs = read_csv_line("input/7_test3.txt", buffer, 1000);
    CHECK_EQUAL(29, num_inputs);

    long expected_buffer[29] = {3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};
    for(int i = 0; i < num_inputs; ++i)
    {
        CHECK_EQUAL(expected_buffer[i], buffer[i]);
    }
}

TEST_GROUP(IO_STR_LINES)
{
};

TEST(IO_STR_LINES, STR_LINES_0)
{
    char buffer[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR];

    int num_lines = read_str_lines("input/10_test0.txt", buffer);

    CHECK_EQUAL(5, num_lines);
    CHECK_EQUAL(0, strcmp(".#..#\n", buffer[0]));
    CHECK_EQUAL(0, strcmp(".....\n", buffer[1]));
    CHECK_EQUAL(0, strcmp("#####\n", buffer[2]));
    CHECK_EQUAL(0, strcmp("....#\n", buffer[3]));
    CHECK_EQUAL(0, strcmp("...##\n", buffer[4]));
}
