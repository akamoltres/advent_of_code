
#include <stdio.h>
#include <string.h>

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

unsigned long part2(char *input_filename)
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

    int field_assigned[MAX_NUM_FIELDS];
    memset(field_assigned, 0, MAX_NUM_FIELDS * sizeof(int));

    int field_used[MAX_NUM_FIELDS];
    memset(field_used, 0, MAX_NUM_FIELDS * sizeof(int));

    int ticket_valid[MAX_NUM_TICKETS];
    int error_rate = 0;

    for(int i = 1; i < num_tickets; ++i)
    {
        int ticket_invalid = 0;
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
                ticket_invalid = 1;
            }
        }
        ticket_valid[i] = !ticket_invalid;
    }

    // is field [subscript0] valid for field [subscript1] on the tickets?
    int is_candidate[MAX_NUM_FIELDS][MAX_NUM_FIELDS];
    memset(is_candidate, 0, MAX_NUM_FIELDS * MAX_NUM_FIELDS * sizeof(int));
    for(int i = 0; i < num_fields; ++i) // is field i...
    {
        for(int j = 0; j < num_fields; ++j) // ...valid for the j'th ticket field?
        {
            is_candidate[i][j] = 1;
            for(int k = 1; k < num_tickets; ++k)
            {
                if(ticket_valid[k])
                {
                    if(!(fields[i][0] <= tickets[k][j] && tickets[k][j] <= fields[i][1]) &&
                       !(fields[i][2] <= tickets[k][j] && tickets[k][j] <= fields[i][3]))
                    {
                        is_candidate[i][j] = 0;
                        break;
                    }
                }
            }
        }
    }

    int remaining_candidates = num_fields;
    while(remaining_candidates > 0)
    {
        // check if there are any ticket fields with just one candidate
        for(int i = 0; i < num_fields; ++i)
        {
            // don't stomp on ticket fields that are taken care of
            if(!field_used[i])
            {
                // count candidates for this ticket field, storing the most recent
                int candidate_count = 0;
                int last_candidate = -1;
                for(int j = 0; j < num_fields && candidate_count < 2; ++j)
                {
                    if(is_candidate[j][i])
                    {
                        candidate_count += 1;
                        last_candidate = j;
                    }
                }

                // only one option
                if(candidate_count == 1)
                {
                    field_assigned[last_candidate] = i;
                    field_used[i] = 1;
                    for(int j = 0; j < num_fields; ++j)
                    {
                        // remove this field from consideration from other ticket fields
                        is_candidate[last_candidate][j] = 0;
                    }
                    remaining_candidates -= 1;
                    break;
                }
            }
        }
    }

    unsigned long product = 1;

    for(int i = 0; i < num_fields; ++i)
    {
        char departure_str[10] = "departure";
        if(strlen(field_names[i]) > strlen(departure_str))
        {
            char test[10];
            memcpy(test, field_names[i], strlen(departure_str));
            test[9] = '\0';
            if(!strcmp(departure_str, test))
            {
                product *= (unsigned long) tickets[0][field_assigned[i]];
            }
        }
    }

    return product;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %lu\n", part2(argv[1]));

    return 0;
}
