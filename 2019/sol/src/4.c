
#include <stdio.h>
#include <stdlib.h>

// Converts num to string in the provided buffer and returns the number of digits
// Returns -1 if failed
int num2str(int num, const int bufsize, char *buffer)
{
    int num_digits = 0;

    do {
        if(num_digits == bufsize)
        {
            return -1;
        }
        buffer[num_digits++] = num % 10;
        num /= 10;
    } while(num > 0);

    for(int i = 0; i < num_digits / 2; ++i)
    {
        char temp = buffer[i];
        buffer[i] = buffer[num_digits - i - 1];
        buffer[num_digits - i - 1] = temp;
    }

    return num_digits;
}

// Returns -1 if failed
int solve_2019_4_1(char const *input_filename)
{
    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    int low, high;
    fscanf(fp, "%d", &low);
    fscanf(fp, "%d", &high);

    fclose(fp);

    int valid_count = 0;

    for(int i = low; i <= high; ++i)
    {
        const int digits = 6;
        char num[digits];

        // Convert to string
        if(num2str(i, digits, num) == -1)
        {
            return -1;
        }

        // Evaluate criteria
        int is_double = 0;
        int is_decrease = 0;
        for(int j = 1; j < digits; ++j)
        {
            if(num[j] == num[j-1])
            {
                is_double = 1;
            }
            else if(num[j] < num[j-1])
            {
                is_decrease = 1;
            }
        }

        // Update result counter
        valid_count += (is_double && !is_decrease);
    }

    return valid_count;
}

// Returns -1 if failed
int solve_2019_4_2(char const *input_filename)
{
    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    int low, high;
    fscanf(fp, "%d", &low);
    fscanf(fp, "%d", &high);

    fclose(fp);

    int valid_count = 0;

    for(int i = low; i <= high; ++i)
    {
        const int digits = 6;
        char num[digits];

        // Convert to string
        int numlen = num2str(i, digits, num);
        if(numlen == -1)
        {
            return -1;
        }

        // Evaluate decreasing criteria
        int is_decrease = 0;
        for(int j = 1; j < digits; ++j)
        {
            if(num[j] < num[j-1])
            {
                is_decrease = 1;
            }
        }

        // Evaluate double criteria and update result counter
        if(num[0] == num[1] && num[1] != num[2])
        {
            valid_count += (!is_decrease);
            continue;
        }
        if(num[numlen - 1] == num[numlen - 2] && num[numlen - 2] != num[numlen - 3])
        {
            valid_count += (!is_decrease);
            continue;
        }
        for(int j = 1; j < 4; ++j)
        {
            if(num[j - 1] != num[j] && num[j] == num[j + 1] && num[j + 1] != num[j + 2])
            {
                valid_count += (!is_decrease);
                break;
            }
        }
    }

    return valid_count;
}

/*int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Exactly 2 arguments (password range) required\n");
        return -1;
    }

    int low = atoi(argv[1]);
    int high = atoi(argv[2]);

    printf("Part 1: %d\n", part1(low, high));
    printf("Part 2: %d\n", part2(low, high));

    return 0;
}*/
