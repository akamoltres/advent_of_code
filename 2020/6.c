
#include <stdio.h>

#define BUFSIZE 1000
#define GROUP_BUFSIZE 100
#define MAX_RESPONSES 27

typedef struct Group
{
    int num_people;
    int response_lengths[GROUP_BUFSIZE];
    char responses[GROUP_BUFSIZE][MAX_RESPONSES];
} Group_t;

// Returns number of groups
// Returns -1 if failed unexpectedly
int get_input(const int bufsize, Group_t *buffer)
{
    int num_groups = 0;

    FILE *fp;
    fp = fopen("6.txt", "r");

    if(fp == NULL)
    {
        return -1;
    }

    // Iterate on the groups
    while(1 != 2)
    {
        if(num_groups == bufsize)
        {
            fclose(fp);
            return -1;
        }

        int *num_people = &buffer[num_groups].num_people;
        *num_people = 0;

        // Iterate on the people in the groups
        while(1 != 2)
        {
            if(*num_people == GROUP_BUFSIZE)
            {
                fclose(fp);
                return -1;
            }

            int *response_length = &buffer[num_groups].response_lengths[buffer[num_groups].num_people];
            *response_length = 0;

            // Get the person's responses
            char *c = buffer[num_groups].responses[*num_people];
            while(1 != 2)
            {
                if(*response_length == MAX_RESPONSES)
                {
                    fclose(fp);
                    return -1;
                }

                char *retval = fgets(c, 2, fp);

                if(*c == '\n')
                {
                    *c = '\0';
                    break;
                }

                *response_length += 1;
                c++;

                if(!retval)
                {
                    fclose(fp);
                    return num_groups + 1;
                }
            }

            // Done with group
            if(*response_length == 0)
            {
                break;
            }

            *num_people += 1;
        }

        num_groups += 1;
    }

    fclose(fp);
    return -1;
}

// Returns -1 if failed unexpectedly
int part1()
{
    const int bufsize = BUFSIZE;
    Group_t buffer[bufsize];

    int num_groups = get_input(bufsize, buffer);
    if(num_groups == -1)
    {
        return -1;
    }

    int sum = 0;

    for(int i = 0; i < num_groups; ++i)
    {
        int responses[MAX_RESPONSES] = {0};

        for(int j = 0; j < buffer[i].num_people; ++j)
        {
            for(int k = 0; k < buffer[i].response_lengths[j]; ++k)
            {
                responses[buffer[i].responses[j][k] - 'a'] = 1;
            }
        }

        for(int j = 0; j < MAX_RESPONSES; ++j)
        {
            sum += responses[j];
        }
    }

    return sum;
}

// Returns -1 if failed unexpectedly
int part2()
{
    const int bufsize = BUFSIZE;
    Group_t buffer[bufsize];

    int num_groups = get_input(bufsize, buffer);
    if(num_groups == -1)
    {
        return -1;
    }

    int sum = 0;

    for(int i = 0; i < num_groups; ++i)
    {
        int responses[MAX_RESPONSES] = {0};

        for(int j = 0; j < buffer[i].num_people; ++j)
        {
            for(int k = 0; k < buffer[i].response_lengths[j]; ++k)
            {
                responses[buffer[i].responses[j][k] - 'a'] += 1;
            }
        }

        for(int j = 0; j < MAX_RESPONSES; ++j)
        {
            if(responses[j] == buffer[i].num_people)
            {
                sum += 1;
            }
        }
    }

    return sum;
}

int main()
{
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
}
