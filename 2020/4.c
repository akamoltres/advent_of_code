
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FIELD_LENGTH 100

typedef struct Passport
{
    int len_byr;
    char byr[MAX_FIELD_LENGTH];
    int len_iyr;
    char iyr[MAX_FIELD_LENGTH];
    int len_eyr;
    char eyr[MAX_FIELD_LENGTH];
    int len_hgt;
    char hgt[MAX_FIELD_LENGTH];
    int len_hcl;
    char hcl[MAX_FIELD_LENGTH];
    int len_ecl;
    char ecl[MAX_FIELD_LENGTH];
    int len_pid;
    char pid[MAX_FIELD_LENGTH];
    int len_cid;
    char cid[MAX_FIELD_LENGTH];
} Passport_t;

// Returns number of passports
// Returns -1 if failed unexpectedly
int get_input(const int bufsize, Passport_t *buffer)
{
    int num_passports = 0;

    FILE *fp;
    fp = fopen("4.txt", "r");

    if(fp == NULL)
    {
        return -1;
    }

    while(1 != 2)
    {
        buffer[num_passports].len_byr = 0;
        buffer[num_passports].len_iyr = 0;
        buffer[num_passports].len_eyr = 0;
        buffer[num_passports].len_hgt = 0;
        buffer[num_passports].len_hcl = 0;
        buffer[num_passports].len_ecl = 0;
        buffer[num_passports].len_pid = 0;
        buffer[num_passports].len_cid = 0;

        if(num_passports == bufsize)
        {
            return -1;
        }

        while(1 != 2)
        {
            // Get the line of input
            int input_buffer_len = 0;
            char input_buffer[MAX_FIELD_LENGTH];
            while(1 != 2)
            {
                if(input_buffer_len == MAX_FIELD_LENGTH)
                {
                    return -1;
                }

                char *retval = fgets(input_buffer + input_buffer_len, 2, fp);

                // Ditch if we've hit EOF
                if(retval == NULL)
                {
                    fclose(fp);
                    num_passports += 1;
                    return num_passports;
                }

                // End of this line
                if(input_buffer[input_buffer_len] == '\n')
                {
                    input_buffer[input_buffer_len] = '\0';
                    break;
                }

                input_buffer_len += 1;
            }

            // Empty line, this passport is done
            if(input_buffer_len == 0)
            {
                break;
            }

            // Break up the string into the tokens
            const char delim[2] = " ";
            char *token = strtok(input_buffer, delim);
            while(token != NULL)
            {
                if(token[0] == 'b' && token[1] == 'y' && token[2] == 'r')
                {
                    buffer[num_passports].len_byr = strlen(token) - 4;
                    strcpy(buffer[num_passports].byr, token + 4);
                }
                else if(token[0] == 'i' && token[1] == 'y' && token[2] == 'r')
                {
                    buffer[num_passports].len_iyr = strlen(token) - 4;
                    strcpy(buffer[num_passports].iyr, token + 4);
                }
                else if(token[0] == 'e' && token[1] == 'y' && token[2] == 'r')
                {
                    buffer[num_passports].len_eyr = strlen(token) - 4;
                    strcpy(buffer[num_passports].eyr, token + 4);
                }
                else if(token[0] == 'h' && token[1] == 'g' && token[2] == 't')
                {
                    buffer[num_passports].len_hgt = strlen(token) - 4;
                    strcpy(buffer[num_passports].hgt, token + 4);
                }
                else if(token[0] == 'h' && token[1] == 'c' && token[2] == 'l')
                {
                    buffer[num_passports].len_hcl = strlen(token) - 4;
                    strcpy(buffer[num_passports].hcl, token + 4);
                }
                else if(token[0] == 'e' && token[1] == 'c' && token[2] == 'l')
                {
                    buffer[num_passports].len_ecl = strlen(token) - 4;
                    strcpy(buffer[num_passports].ecl, token + 4);
                }
                else if(token[0] == 'p' && token[1] == 'i' && token[2] == 'd')
                {
                    buffer[num_passports].len_pid = strlen(token) - 4;
                    strcpy(buffer[num_passports].pid, token + 4);
                }
                else if(token[0] == 'c' && token[1] == 'i' && token[2] == 'd')
                {
                    buffer[num_passports].len_cid = strlen(token) - 4;
                    strcpy(buffer[num_passports].cid, token + 4);
                }

                token = strtok(NULL, delim);
            }
        }

        num_passports += 1;
    }
}

// Returns -1 if failed unexpectedly
int part1()
{
    const int bufsize = 5000;
    Passport_t buffer[bufsize];

    int num_passports = get_input(bufsize, buffer);
    if(num_passports == -1)
    {
        return -1;
    }

    int valid_count = 0;

    for(int i = 0; i < num_passports; ++i)
    {
        if(buffer[i].len_byr != 0 && 
           buffer[i].len_iyr != 0 && 
           buffer[i].len_eyr != 0 && 
           buffer[i].len_hgt != 0 && 
           buffer[i].len_hcl != 0 && 
           buffer[i].len_ecl != 0 && 
           buffer[i].len_pid != 0)
        {
            valid_count += 1;
        }
    }

    return valid_count;
}

// Returns -1 if failed unexpectedly
int part2()
{
    const int bufsize = 5000;
    Passport_t buffer[bufsize];

    int num_passports = get_input(bufsize, buffer);
    if(num_passports == -1)
    {
        return -1;
    }

    int valid_count = 0;

    for(int i = 0; i < num_passports; ++i)
    {
        if(buffer[i].len_byr != 4 ||
           buffer[i].len_iyr != 4 ||
           buffer[i].len_eyr != 4 ||
          (buffer[i].len_hgt != 4 && buffer[i].len_hgt != 5) ||
           buffer[i].len_hcl != 7 ||
           buffer[i].len_ecl != 3 ||
           buffer[i].len_pid != 9)
        {
            continue;
        }

        int byr = atoi(buffer[i].byr);
        if(byr < 1920 || 2002 < byr)
        {
            continue;
        }

        int iyr = atoi(buffer[i].iyr);
        if(iyr < 2010 || 2020 < iyr)
        {
            continue;
        }

        int eyr = atoi(buffer[i].eyr);
        if(eyr < 2020 || 2030 < eyr)
        {
            continue;
        }

        int hgt = atoi(buffer[i].hgt);
        if(hgt < 59 || (76 < hgt && hgt < 150) || 193 < hgt)
        {
            continue;
        }
        if(59 <= hgt && hgt <= 76)
        {
            if(buffer[i].len_hgt != 4)
            {
                continue;
            }
            if(buffer[i].hgt[2] != 'i' || buffer[i].hgt[3] != 'n')
            {
                continue;
            }
        }
        if(150 <= hgt && hgt <= 193)
        {
            if(buffer[i].len_hgt != 5)
            {
                continue;
            }
            if(buffer[i].hgt[3] != 'c' || buffer[i].hgt[4] != 'm')
            {
                continue;
            }
        }

        if(buffer[i].hcl[0] != '#')
        {
            continue;
        }
        int fail = 0;
        for(int j = 1; j <= 6 && fail == 0; ++j)
        {
            if(!isdigit(buffer[i].hcl[j]) && !('a' <= buffer[i].hcl[j] && buffer[i].hcl[j] <= 'f'))
            {
                fail = 1;
            }
        }
        if(fail)
        {
            continue;
        }

        if(strcmp(buffer[i].ecl, "amb") &&
           strcmp(buffer[i].ecl, "blu") &&
           strcmp(buffer[i].ecl, "brn") &&
           strcmp(buffer[i].ecl, "gry") &&
           strcmp(buffer[i].ecl, "grn") &&
           strcmp(buffer[i].ecl, "hzl") &&
           strcmp(buffer[i].ecl, "oth"))
        {
            continue;
        }

        fail = 0;
        for(int j = 0; j < 9; ++j)
        {
            if(!isdigit(buffer[i].pid[j]))
            {
                fail = 0;
            }
        }
        if(fail)
        {
            continue;
        }

        valid_count += 1;
    }

    return valid_count;
}

int main()
{
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
}
