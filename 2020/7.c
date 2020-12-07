
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1000
#define QUEUE_SIZE 1000000
#define MAX_DESC_LENGTH 20

typedef struct Description
{
    char adj[MAX_DESC_LENGTH];
    char col[MAX_DESC_LENGTH];
} Description_t;

// Returns -1 if the bag is not in the key already
int index_of(int num_bag_types, Description_t key[BUFSIZE], Description_t *test)
{
    for(int i = 0; i < num_bag_types; ++i)
    {
        if(!strcmp(key[i].adj, test->adj) && !strcmp(key[i].col, test->col))
        {
            return i;
        }
    }

    // Not found
    return -1;
}

// Returns -1 if invalid input
int read_input(Description_t key[BUFSIZE], int count[BUFSIZE][BUFSIZE])
{
    int num_bag_types = 0;

    FILE *fp;
    fp = fopen("7.txt", "r");

    if(fp == NULL)
    {
        return -1;
    }

    int retval;
    char input_buffer[MAX_DESC_LENGTH];

    while(1 != 2)
    {
        if(num_bag_types == BUFSIZE)
        {
            return -1;
        }

        Description_t this_bag;

        // Adjective
        retval = fscanf(fp, "%s", this_bag.adj);
        if(retval == EOF)
        {
            fclose(fp);
            return num_bag_types;
        }

        // Color
        (void) fscanf(fp, "%s", this_bag.col);

        // Add the bag if it has not been added
        int index = index_of(num_bag_types, key, &this_bag);
        if(index == -1)
        {
            key[num_bag_types] = this_bag;
            index = num_bag_types++;
        }

        // Throw away "bags contain"
        (void) fscanf(fp, "%s", input_buffer);
        (void) fscanf(fp, "%s", input_buffer);

        // Get the contents
        while(1 != 2)
        {
            if(num_bag_types == BUFSIZE)
            {
                fclose(fp);
                return -1;
            }

            // Get the next bag count
            int contents_count;
            retval = fscanf(fp, "%d", &contents_count);
            (void) fscanf(fp, "%s", this_bag.adj);
            (void) fscanf(fp, "%s", this_bag.col);
            (void) fscanf(fp, "%s", input_buffer);

            // Contains no other bags
            if(retval == 0)
            {
                break;
            }

            // Add this information
            int contents_index = index_of(num_bag_types, key, &this_bag);
            if(contents_index == -1)
            {
                key[num_bag_types] = this_bag;
                contents_index = num_bag_types++;
            }
            count[index][contents_index] = contents_count;

            // Check if it's the last one
            if(input_buffer[3] == '.' || input_buffer[4] == '.')
            {
                break;
            }
        }
    }

    fclose(fp);
    return -1;
}

// Returns -1 if failure
int push_queue(int *queue_length, int queue[QUEUE_SIZE], int to_push)
{
    if(*queue_length == QUEUE_SIZE)
    {
        return -1;
    }

    queue[(*queue_length)++] = to_push;

    return 0;
}

// Returns -1 if failure
int pop_queue(int *queue_length, int queue[QUEUE_SIZE], int index)
{
    if(*queue_length < 1)
    {
        return -1;
    }

    int value = queue[index];

    for(int i = index; i < *queue_length - 1; ++i)
    {
        queue[i] = queue[i + 1];
    }

    *queue_length -= 1;

    return value;
}

// Returns -1 if failed unexpectedly
int part1()
{
    Description_t key[BUFSIZE];
    int count[BUFSIZE][BUFSIZE] = {0};

    int num_bag_types = read_input(key, count);
    if(num_bag_types == -1)
    {
        return -1;
    }

    // Get the starting point
    Description_t shiny_gold = {
        .adj = "shiny",
        .col = "gold",
    };
    int index_shiny_gold = index_of(num_bag_types, key, &shiny_gold);

    // Initialize queue
    int queue_length = 1;
    int queue[QUEUE_SIZE] = {0};
    queue[0] = index_shiny_gold;

    // Walk through the tree of things containing gold until they're exhausted
    while(queue_length > 0)
    {
        int current_index = pop_queue(&queue_length, queue, 0);

        // For every bag that contains this bag...add this bag's contents
        // to that bag
        for(int i = 0; i < num_bag_types; ++i)
        {
            // I refuse to believe a bag can contain itself
            if(i == current_index)
            {
                continue;
            }

            if(count[i][current_index])
            {
                for(int j = 0; j < num_bag_types; ++j)
                {
                    count[i][j] += count[i][current_index] * count[current_index][j];
                }

                if(push_queue(&queue_length, queue, i))
                {
                    return -1;
                }
            }
        }
    }

    // Count the bags that contain shiny gold
    int shiny_gold_count = 0;
    for(int i = 0; i < num_bag_types; ++i)
    {
        if(count[i][index_shiny_gold])
        {
            shiny_gold_count += 1;
        }
    }

    return shiny_gold_count;
}

int main()
{
    printf("Part 1: %d\n", part1());
}
