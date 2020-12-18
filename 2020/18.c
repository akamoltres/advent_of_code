
#include <assert.h>
#include <stdio.h>

typedef unsigned long ul;

int EOF_FLAG;

ul apply_op(char op, ul a, ul b)
{
    switch(op)
    {
        case '+':
            return (a + b);
        case '-':
            return (a - b);
        case '*':
            return (a * b);
        case '/':
            return (a / b);
    }

    // should not reach here
    assert(0);
}

ul compute_expression(FILE *fp)
{
    EOF_FLAG = 0;

    ul so_far = 0;
    int first = 1;
    char op = '\0';

    while(1)
    {
        char c[2];
        char *retval = fgets(c, 2, fp);

        if(retval == NULL)
        {
            EOF_FLAG = 1;
            return 0;
        }

        switch(c[0])
        {
            case '\n':
                return so_far;
            case ' ':
                break;
            case '(':
                if(first)
                {
                    so_far = compute_expression(fp);
                }
                else
                {
                    so_far = apply_op(op, so_far, compute_expression(fp));
                }
                break;
            case ')':
                return so_far;
            case '0' ... '9':
                if(first)
                {
                    so_far = (c[0] - '0');
                }
                else
                {
                    so_far = apply_op(op, so_far, c[0] - '0');
                }
                break;
            default: // it is an operator
                op = c[0];
                break;
        }

        first = 0;
    }

    // should not reach here
    assert(0);
}

ul part1(char *input_filename)
{
    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    ul total_sum = 0;

    do {
        ul value = compute_expression(fp);
        total_sum += value;
    } while(!EOF_FLAG);

    fclose(fp);

    return total_sum;
}

ul part2(char *input_filename)
{
    return -1;
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
