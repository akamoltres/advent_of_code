
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 1000
#define PASS_LENGTH 10

// Returns number of boarding passes
// Returns -1 if failed unexpectedly
int get_input(const int bufsize, char buffer[BUFSIZE][PASS_LENGTH])
{
    int num_passes = 0;

    FILE *fp;
    fp = fopen("5.txt", "r");

    if(fp == NULL)
    {
        return -1;
    }

    while(fscanf(fp, "%s", buffer[num_passes++]) != EOF)
    {
        if(num_passes == bufsize)
        {
            num_passes = -1;
            return num_passes;
        }
    }

    return num_passes - 1;
}

int get_seat_id(char *pass)
{
    int rlow = 0;
    int rhigh = 127;
    int clow = 0;
    int chigh = 7;

    for(int i = 0; i < 10; ++i)
    {
        if(pass[i] == 'F')
        {
            rhigh -= (rhigh - rlow + 1) / 2;
        }
        else if(pass[i] == 'B')
        {
            rlow += (rhigh - rlow + 1) / 2;
        }
        else if(pass[i] == 'L')
        {
            chigh -= (chigh - clow + 1) / 2;
        }
        else if(pass[i] == 'R')
        {
            clow += (chigh - clow + 1) / 2;
        }
    }

    return (rlow * 8 + clow);
}

// Returns -1 if failed unexpectedly
int part1()
{
    const int bufsize = BUFSIZE;
    char buffer[bufsize][PASS_LENGTH];

    int num_passes = get_input(bufsize, buffer);
    if(num_passes == -1)
    {
        return -1;
    }

    int highest_seat_id = 0;

    for(int i = 0; i < num_passes; ++i)
    {
        int seat_id = get_seat_id(buffer[i]);
        highest_seat_id = (highest_seat_id < seat_id ? seat_id : highest_seat_id);
    }

    return highest_seat_id;
}

// Returns -1 if failed unexpectedly
int part2()
{
    const int bufsize = BUFSIZE;
    char buffer[bufsize][PASS_LENGTH];

    int num_passes = get_input(bufsize, buffer);
    if(num_passes == -1)
    {
        return -1;
    }

    int highest_seat_id = 0;
    int filled[BUFSIZE] = {0};
    for(int i = 0; i < num_passes; ++i)
    {
        int seat_id = get_seat_id(buffer[i]);
        highest_seat_id = (highest_seat_id < seat_id ? seat_id : highest_seat_id);
        filled[seat_id] = 1;
    }

    // Assumes the seat is not in the second to last row
    // coupled with the last few seats missing
    for(int i = 8; i < highest_seat_id - 8; ++i)
    {
        if(!filled[i] && filled[i-1] && filled[i+1])
        {
            return i;
        }
    }

    return -1;
}

int main()
{
    printf("Part 1: %d\n", part1());
    printf("Part 2: %d\n", part2());
}
