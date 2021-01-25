
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
