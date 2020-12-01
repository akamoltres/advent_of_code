
#include <stdio.h>

void print_program(int program_length, int *buffer)
{
    if(program_length > 0)
    {
        printf("%d\n", buffer[0]);
        for(int i = 1; i < program_length; ++i)
        {
            printf(" %d", buffer[i]);
        }
        printf("\n");
    }
}

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
        int negative = 0;
        while(fgets(c, 2, fp))
        {
            if(c[0] == '-')
            {
                negative = 1;
                continue;
            }

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

        buffer[program_length++] = value * (negative ? -1 : 1);

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

int run_intcode(const int program_length, const int bufsize, int *buffer, const int input_length, int *input_buffer)
{
    int position = 0;
    int current_input = 0;

    while(buffer[position] != 99)
    {
        int mode1 = (buffer[position] % 1000) / 100;
        int mode2 = (buffer[position] % 10000) / 1000;
        switch(buffer[position] % 100)
        {
            case 1: // add
            {
                int param1 = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                int param2 = (mode2 ? buffer[position + 2] : buffer[buffer[position + 2]]);
                buffer[buffer[position + 3]] = param1 + param2;
                position += 4;
                break;
            }
            case 2: // multiply
            {
                int param1 = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                int param2 = (mode2 ? buffer[position + 2] : buffer[buffer[position + 2]]);
                buffer[buffer[position + 3]] = param1 * param2;
                position += 4;
                break;
            }
            case 3: // input
            {
                if(current_input == input_length)
                {
                    return -1;
                }
                buffer[buffer[position + 1]] = input_buffer[current_input++];
                position += 2;
                break;
            }
            case 4: // output
            {
                int param = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                printf("Intcode program output: %d\n", param);
                position += 2;
                break;
            }
            case 5: // jump if true
            {
                int param1 = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                int param2 = (mode2 ? buffer[position + 2] : buffer[buffer[position + 2]]);
                if(param1)
                {
                    position = param2;
                }
                else
                {
                    position += 3;
                }
                break;
            }
            case 6: // jump if false
            {
                int param1 = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                int param2 = (mode2 ? buffer[position + 2] : buffer[buffer[position + 2]]);
                if(!param1)
                {
                    position = param2;
                }
                else
                {
                    position += 3;
                }
                break;
            }
            case 7: // less than
            {
                int param1 = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                int param2 = (mode2 ? buffer[position + 2] : buffer[buffer[position + 2]]);
                buffer[buffer[position + 3]] = (param1 < param2);
                position += 4;
                break;
            }
            case 8: // equals
            {
                int param1 = (mode1 ? buffer[position + 1] : buffer[buffer[position + 1]]);
                int param2 = (mode2 ? buffer[position + 2] : buffer[buffer[position + 2]]);
                buffer[buffer[position + 3]] = (param1 == param2);
                position += 4;
                break;
            }
            default:
            {
                return -1;
            }
        }

        if(position >= bufsize)
        {
            return -1;
        }
    }

    return 0;
}
