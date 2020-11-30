
#include <stdio.h>

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
int part1(int low, int high)
{
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

int main()
{
    int low = 165432;
    int high = 707912;
    printf("Part 1: %d\n", part1(low, high));
    //printf("Part 2: %d\n", part2());
    return 0;
}