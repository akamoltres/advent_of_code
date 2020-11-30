
#include <stdio.h>
#include <stdlib.h>

typedef struct Segment {
    int x1, y1;
    int x2, y2;
} Segment_t;

// Returns number of wire segments if success
// Returns -1 if the buffer is not long enough
// Returns -2 for any other errors
int read_wires(const int bufsize, Segment_t *buffer, FILE *fp)
{
    int num_segments = 0;
    int x_prev = 0;
    int y_prev = 0;

    int end_of_wire = 0;
    while(1)
    {
        char c[2];
        char direction;
        int length = 0;

        // Get the direction
        fgets(c, 2, fp);
        direction = c[0];

        // Get the segment length and check for end of wire
        while(fgets(c, 2, fp))
        {
            if(c[0] == ',')
            {
                break;
            }

            if(c[0] == '\n' || feof(fp))
            {
                end_of_wire = 1;
                break;
            }

            length = (length * 10) + (c[0] - '0');
        }

        buffer[num_segments].x1 = x_prev;
        buffer[num_segments].y1 = y_prev;

        switch(direction)
        {
        case 'U':
            buffer[num_segments].x2 = x_prev;
            buffer[num_segments].y2 = y_prev + length;
            break;
        case 'D':
            buffer[num_segments].x2 = x_prev;
            buffer[num_segments].y2 = y_prev - length;
            break;
        case 'R':
            buffer[num_segments].x2 = x_prev + length;
            buffer[num_segments].y2 = y_prev;
            break;
        case 'L':
            buffer[num_segments].x2 = x_prev - length;
            buffer[num_segments].y2 = y_prev;
            break;
        default:
            return -2;
        }

        x_prev = buffer[num_segments].x2;
        y_prev = buffer[num_segments].y2;

        num_segments += 1;

        if(num_segments == bufsize)
        {
            return -1;
        }

        if(end_of_wire)
        {
            return num_segments;
        }
    }
}

// Returns Manhattan distance if segments cross, -1 otherwise
int wires_crossed(Segment_t segment1, Segment_t segment2)
{
    // If both are horizontal or both are vertical, they cannot cross
    if((segment1.x2 - segment1.x1 == 0 && segment2.x2 - segment2.x1 == 0) ||
       (segment1.y2 - segment1.y1 == 0 && segment2.y2 - segment2.y1 == 0))
    {
        return -1;
    }

    // If segment1 is entirely above or below segment2, they cannot cross
    if((segment1.y1 > segment2.y1 && segment1.y2 > segment2.y1 &&
        segment1.y1 > segment2.y2 && segment1.y2 > segment2.y2) ||
       (segment1.y1 < segment2.y1 && segment1.y2 < segment2.y1 &&
        segment1.y1 < segment2.y2 && segment1.y2 < segment2.y2))
    {
        return -1;
    }

    // If segment1 is entirely to the left or right of segment2, they cannot cross
    if((segment1.x1 > segment2.x1 && segment1.x2 > segment2.x1 &&
        segment1.x1 > segment2.x2 && segment1.x2 > segment2.x2) ||
       (segment1.x1 < segment2.x1 && segment1.x2 < segment2.x1 &&
        segment1.x1 < segment2.x2 && segment1.x2 < segment2.x2))
    {
        return -1;
    }

    // They must cross
    if(segment1.x1 == segment1.x2) // segment1 is vertical
    {
        return (abs(segment1.x1) + abs(segment2.y1));
    }
    else // segment1 is horizontal
    {
        return (abs(segment1.y1) + abs(segment2.x1));
    }
}

// Returns -1 if failed
int part1()
{
    const int bufsize = 500;
    Segment_t wire1[bufsize];
    Segment_t wire2[bufsize];

    FILE *fp;
    fp = fopen("3.txt", "r");

    if(fp == NULL)
    {
        return -1;
    }

    int wire1_length = read_wires(bufsize, wire1, fp);
    if(wire1_length < 0)
    {
        printf("Wire 1 read failed with error code %d\n", wire1_length);
        return -1;
    }

    int wire2_length = read_wires(bufsize, wire2, fp);
    if(wire2_length < 0)
    {
        printf("Wire 2 read failed with error code %d\n", wire2_length);
        return -1;
    }

    fclose(fp);

    int min_manhattan_distance = -1;
    for(int i = 0; i < wire1_length; ++i)
    {
        for(int j = 0; j < wire2_length; ++j)
        {
            // First two segments, by definition, two not cross
            if(i == 0 && j == 0)
            {
                continue;
            }

            int manhattan_distance = wires_crossed(wire1[i], wire2[j]);

            if(manhattan_distance >= 0 &&
              (min_manhattan_distance == -1 || manhattan_distance < min_manhattan_distance))
            {
                min_manhattan_distance = manhattan_distance;
            }
        }
    }

    return min_manhattan_distance;
}

int main()
{
    printf("Part 1: %d\n", part1());
}
