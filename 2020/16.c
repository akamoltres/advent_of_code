
#include <stdio.h>

#define FIELD_LENGTH 100
#define MAX_NUM_TICKETS 1000
#define MAX_NUM_FIELDS 100

int read_uint(FILE *fp)
{
    int n = 0;
    if(fscanf(fp, "%d", &n) != EOF)
    {
        return n;
    }
    return -1;
}

int read_csv_line(FILE *fp, int length, int *array)
{
    char c[2];
    for(int i = 0; i < length; ++i)
    {
        array[i] = read_uint(fp);
        fgets(c, 2, fp);
        if(array[i] == -1)
        {
            return -1;
        }
    }
    return 0;
}

int get_input(char *input_filename,
              int *num_fields, char field_names[MAX_NUM_FIELDS][FIELD_LENGTH], int fields[MAX_NUM_FIELDS][4],
              int *num_tickets, int tickets[MAX_NUM_TICKETS][MAX_NUM_FIELDS])
{
    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    char c[2];

    // get field constraints
    *num_fields = 0;
    while(1)
    {
        if(*num_fields == MAX_NUM_FIELDS)
        {
            return -1;
        }

        int all_fields_read = 0;

        // read the name
        int idx = 0;
        while(fgets(c, 2, fp))
        {
            if(idx == 0 && c[0] == '\n')
            {
                all_fields_read = 1;
                break;
            }

            if(idx == FIELD_LENGTH)
            {
                return -1;
            }

            if(c[0] == ':')
            {
                field_names[*num_fields][idx] = '\0';
                // throw away the space
                fgets(c, 2, fp);
                break;
            }

            field_names[*num_fields][idx++] = c[0];
        }

        if(all_fields_read)
        {
            break;
        }

        // get the two ranges
        fields[*num_fields][0] = read_uint(fp);
        fgets(c, 2, fp);    // get rid of "-"
        fields[*num_fields][1] = read_uint(fp);
        fgets(c, 2, fp);    // get rid of "or "
        fgets(c, 2, fp);
        fgets(c, 2, fp);
        fgets(c, 2, fp);
        fields[*num_fields][2] = read_uint(fp);
        fgets(c, 2, fp);
        fields[*num_fields][3] = read_uint(fp);

        // throw away the newline
        fgets(c, 2, fp);

        *num_fields += 1;
    }

    // throw away the line that says "your ticket:\n"
    for(int i = 0; i < 13; ++i)
    {
        fgets(c, 2, fp);
    }

    // read your ticket
    read_csv_line(fp, *num_fields, tickets[0]);

    // throw away the line that says "nearby tickets:\n"
    for(int i = 0; i < 17; ++i)
    {
        fgets(c, 2, fp);
    }

    // read nearby tickets
    *num_tickets = 1;
    while(1)
    {
        if(*num_tickets == MAX_NUM_TICKETS)
        {
            return -1;
        }

        int retval = read_csv_line(fp, *num_fields, tickets[*num_tickets]);

        if(retval == -1)
        {
            break;
        }
        
        *num_tickets += 1;
    }

    fclose(fp);

    return 0;
}

int part1(char *input_filename)
{
    int num_fields;
    char field_names[MAX_NUM_FIELDS][FIELD_LENGTH];
    int fields[MAX_NUM_FIELDS][4];

    int num_tickets;
    int tickets[MAX_NUM_TICKETS][MAX_NUM_FIELDS];

    int retval = get_input(input_filename, &num_fields, field_names, fields, &num_tickets, tickets);
    if(retval != 0)
    {
        return retval;
    }

    int error_rate = 0;

    for(int i = 1; i < num_tickets; ++i)
    {
        for(int k = 0; k < num_fields; ++k)
        {
            int invalid = 1;
            for(int j = 0; j < num_fields; ++j)
            {
                if((fields[j][0] <= tickets[i][k] && tickets[i][k] <= fields[j][1]) ||
                   (fields[j][2] <= tickets[i][k] && tickets[i][k] <= fields[j][3]))
                {
                    invalid = 0;
                    break;
                }
            }
            if(invalid)
            {
                error_rate += tickets[i][k];
            }
        }
    }

    return error_rate;
}

int part2(char *input_filename)
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

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
