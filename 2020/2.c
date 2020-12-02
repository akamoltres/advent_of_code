
#include <stdio.h>

#define MAX_PW_LENGTH 100

typedef struct Password
{
    int min;
    int max;
    char letter;
    int pw_length;
    char pw[MAX_PW_LENGTH];
} Password_t;

// Returns number of passwords
// Returns -1 if unexpected failure
int read_passwords(const int bufsize, Password_t *buffer)
{
    int num_passwords = 0;

    FILE *fp;

    fp = fopen("2.txt", "r");
    if(fp == NULL)
    {
        return -1;
    }

    while(1 != 2)
    {
        if(num_passwords == bufsize)
        {
            return -1;
        }

        Password_t *current_pw = buffer + num_passwords;
        char c[4];
        int end_flag = 1;

        // Get min
        current_pw->min = 0;
        while(fgets(c, 2, fp))
        {
            end_flag = 0;

            if(c[0] == '-')
            {
                break;
            }

            current_pw->min = (current_pw->min * 10) + (c[0] - '0');
        }

        // Check if we're done
        if(end_flag)
        {
            return num_passwords;
        }

        // Get max
        current_pw->max = 0;
        while(fgets(c, 2, fp))
        {
            if(c[0] == ' ')
            {
                break;
            }

            current_pw->max = (current_pw->max * 10) + (c[0] - '0');
        }

        // Get letter
        fgets(c, 4, fp);
        current_pw->letter = c[0];

        // Get password
        current_pw->pw_length = 0;
        while(fgets(c, 2, fp))
        {
            if(c[0] == '\n')
            {
                break;
            }

            if(current_pw->pw_length == MAX_PW_LENGTH)
            {
                return -1;
            }

            current_pw->pw[current_pw->pw_length++] = c[0];
        }


        num_passwords += 1;
    }

    fclose(fp);

    return num_passwords;
}

// Returns -1 for unexpected failure
int part1()
{
    const int bufsize = 5000;
    Password_t buffer[bufsize];

    int num_passwords = read_passwords(bufsize, buffer);

    if(num_passwords == -1)
    {
        return -1;
    }

    int num_valid = 0;

    for(int i = 0; i < num_passwords; ++i)
    {
        int counter = 0;
        
        for(int j = 0; j < buffer[i].pw_length; ++j)
        {
            if(buffer[i].pw[j] == buffer[i].letter)
            {
                counter += 1;
            }
        }

        if(buffer[i].min <= counter && counter <= buffer[i].max)
        {
            num_valid += 1;
        }
    }

    return num_valid;
}

// Returns -1 for unexpected failure
int part2()
{
    const int bufsize = 5000;
    Password_t buffer[bufsize];

    int num_passwords = read_passwords(bufsize, buffer);

    if(num_passwords == -1)
    {
        return -1;
    }

    int num_valid = 0;

    for(int i = 0; i < num_passwords; ++i)
    {
        int min = buffer[i].min - 1;
        int max = buffer[i].max - 1;

        if((buffer[i].pw[min] == buffer[i].letter && buffer[i].pw[max] != buffer[i].letter) ||
           (buffer[i].pw[min] != buffer[i].letter && buffer[i].pw[max] == buffer[i].letter))
        {
            num_valid += 1;
        }
    }

    return num_valid;
}

int main()
{
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
}
