
#include "io.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_csv_line(const char *filename, long *buffer, int bufsize)
{
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);

    char *c_buffer;
    size_t c_bufsize = 0;
    assert(getline(&c_buffer, &c_bufsize, fp) != -1);

    fclose(fp);

    int count = 0;

    char *tok;
    tok = strtok(c_buffer, ", \n");
    while(tok != NULL)
    {
        assert(count < bufsize);
        buffer[count++] = atoi(tok);
        tok = strtok(NULL, ", \n");
    }

    free(c_buffer);

    return count;
}

int read_str_lines(const char *filename, char array_in[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR])
{
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);

    memset(array_in, 0, MAX_DIM_STR_ARR * MAX_DIM_STR_ARR * sizeof(char));

    char *input_buffer;
    size_t input_bufsize = 0;
    int num_lines = 0;

    while(getline(&input_buffer, &input_bufsize, fp) != -1)
    {
        assert(strlen(input_buffer) < MAX_DIM_STR_ARR);
        assert(num_lines < MAX_DIM_STR_ARR);
        memcpy(array_in[num_lines++], input_buffer, strlen(input_buffer) * sizeof(char));
    }

    fclose(fp);

    return num_lines;
}
