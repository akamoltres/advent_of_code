
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 2000

typedef struct Action
{
    char dir;
    int distance;
} Action_t;

int get_input(char *input_filename, Action_t buffer[BUFSIZE])
{
    int num_actions = 0;

    FILE *fp = fopen(input_filename, "r");
    if(fp == NULL)
    {
        return -1;
    }

    char line[100];
    while(fscanf(fp, "%s", line) != EOF)
    {
        if(num_actions == BUFSIZE)
        {
            fclose(fp);
            return -1;
        }
        buffer[num_actions].dir = line[0];
        buffer[num_actions].distance = atoi(line + 1);
        num_actions += 1;
    }

    fclose(fp);

    return num_actions;
}

int part1(char *input_filename)
{
    Action_t buffer[BUFSIZE];

    int num_actions = get_input(input_filename, buffer);
    if(num_actions == -1)
    {
        return -1;
    }

    int x = 0;
    int y = 0;
    int angle = 0;
    for(int i = 0; i < num_actions; ++i)
    {
        switch(buffer[i].dir)
        {
            case 'N':
                y += buffer[i].distance;
                break;
            case 'S':
                y -= buffer[i].distance;
                break;
            case 'E':
                x += buffer[i].distance;
                break;
            case 'W':
                x -= buffer[i].distance;
                break;
            case 'L':
                angle += buffer[i].distance;
                break;
            case 'R':
                angle -= buffer[i].distance;
                break;
            case 'F':
                switch(angle)
                {
                    case 0:
                        x += buffer[i].distance;
                        break;
                    case 90:
                        y += buffer[i].distance;
                        break;
                    case 180:
                        x -= buffer[i].distance;
                        break;
                    case 270:
                        y -= buffer[i].distance;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }

        angle = (angle + 360) % 360;
    }

    return (abs(x) + abs(y));
}

int part2(char *input_filename)
{
    Action_t buffer[BUFSIZE];

    int num_actions = get_input(input_filename, buffer);
    if(num_actions == -1)
    {
        return -1;
    }

    int sx = 0;
    int sy = 0;
    int wx = 10;
    int wy = 1;
    for(int i = 0; i < num_actions; ++i)
    {
        int dx0, dy0, dx, dy;
        switch(buffer[i].dir)
        {
            case 'N':
                wy += buffer[i].distance;
                break;
            case 'S':
                wy -= buffer[i].distance;
                break;
            case 'E':
                wx += buffer[i].distance;
                break;
            case 'W':
                wx -= buffer[i].distance;
                break;
            case 'L':
                while(buffer[i].distance > 0)
                {
                    dx0 = wx - sx;
                    dy0 = wy - sy;

                    dx = dy0 * -1;
                    dy = dx0;

                    wx = sx + dx;
                    wy = sy + dy;

                    buffer[i].distance -= 90;
                }
                break;
            case 'R':
                while(buffer[i].distance > 0)
                {
                    dx0 = wx - sx;
                    dy0 = wy - sy;

                    dx = dy0;
                    dy = dx0 * -1;

                    wx = sx + dx;
                    wy = sy + dy;

                    buffer[i].distance -= 90;
                }
                break;
            case 'F':
                dx = wx - sx;
                dy = wy - sy;
                for(int j = 0; j < buffer[i].distance; ++j)
                {
                    sx += dx;
                    sy += dy;
                    wx += dx;
                    wy += dy;
                }
            default:
                break;
        }
    }

    return (abs(sx) + abs(sy));
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
