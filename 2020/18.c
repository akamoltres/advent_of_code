
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef unsigned long ul;

int EOF_FLAG;

ul apply_op(char op, ul a, ul b)
{
    switch(op)
    {
        case '+':
            return (a + b);
        case '*':
            return (a * b);
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

typedef struct Item
{
    int type; // 0 is operator, 1 is number
    char op;
    ul num;
} Item_t;

void apply_ops(char op, int num_items, Item_t *buffer, int *in_use)
{
    for(int i = 0; i < num_items; ++i)
    {
        if(in_use[i] && buffer[i].type == 0 && buffer[i].op == op)
        {
            ul a;
            for(int j = i - 1; j >= 0; --j)
            {
                if(in_use[j] && buffer[j].type == 1)
                {
                    a = buffer[j].num;
                    in_use[j] = 0;
                    break;
                }
            }

            ul b;
            for(int j = i + 1; j < num_items; ++j)
            {
                if(in_use[j] && buffer[j].type == 1)
                {
                    b = buffer[j].num;
                    in_use[j] = 0;
                    break;
                }
            }

            buffer[i].type = 1;
            buffer[i].num = apply_op(op, a, b);
        }
    }
}

ul compute_advanced_expression(FILE *fp)
{
    EOF_FLAG = 0;

    const int bufsize = 100;
    Item_t buffer[bufsize];
    int in_use[bufsize];
    memset(in_use, 0, bufsize * sizeof(int));
    int num_items = 0;

    // get the expression
    char c[2];
    while(1)
    {
        char *retval = fgets(c, 2, fp);

        if(retval == NULL)
        {
            EOF_FLAG = 1;
            return 0;
        }

        int done = 0;

        switch(c[0])
        {
            case '\n':
                done = 1;
                break;
            case ' ':
                break;
            case '(':
                buffer[num_items].type = 1;
                buffer[num_items].num = compute_advanced_expression(fp);
                in_use[num_items] = 1;
                num_items += 1;
                break;
            case ')':
                done = 1;
                break;
            case '0' ... '9':
                buffer[num_items].type = 1;
                buffer[num_items].num = (c[0] - '0');
                in_use[num_items] = 1;
                num_items += 1;
                break;
            default:
                buffer[num_items].type = 0;
                buffer[num_items].op = c[0];
                in_use[num_items] = 1;
                num_items += 1;
                break;
        }

        if(done)
        {
            break;
        }
    }

    // run through addition
    apply_ops('+', num_items, buffer, in_use);

    // run through multiplication
    apply_ops('*', num_items, buffer, in_use);

    // there should be one value remaining
    ul retval;
    int found = 0;
    for(int i = 0; i < num_items; ++i)
    {
        if(in_use[i])
        {
            assert(!found);              // only one value should be left
            assert(buffer[i].type == 1); // and that value should be a number
            retval = buffer[i].num;
            found = 1;
        }
    }

    return retval;
}

ul part2(char *input_filename)
{
    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    ul total_sum = 0;
    do {
        ul value = compute_advanced_expression(fp);
        total_sum += value;
    } while(!EOF_FLAG);

    return total_sum;
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
