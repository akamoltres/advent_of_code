
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Point {
    int x, y;
} Point_t;

typedef struct Segment {
    Point_t p1, p2;
    char direction;
    int length;
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

        buffer[num_segments].direction = direction;
        buffer[num_segments].length = length;
        buffer[num_segments].p1.x = x_prev;
        buffer[num_segments].p1.y = y_prev;

        switch(direction)
        {
        case 'U':
            buffer[num_segments].p2.x = x_prev;
            buffer[num_segments].p2.y = y_prev + length;
            break;
        case 'D':
            buffer[num_segments].p2.x = x_prev;
            buffer[num_segments].p2.y = y_prev - length;
            break;
        case 'R':
            buffer[num_segments].p2.x = x_prev + length;
            buffer[num_segments].p2.y = y_prev;
            break;
        case 'L':
            buffer[num_segments].p2.x = x_prev - length;
            buffer[num_segments].p2.y = y_prev;
            break;
        default:
            return -2;
        }

        x_prev = buffer[num_segments].p2.x;
        y_prev = buffer[num_segments].p2.y;

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

// If segment intersects, returns Manhattan distance and sets intersection point
// If not, returns -1 and does not touch intersection point
int segment_intersection(Segment_t segment1, Segment_t segment2, Point_t *intersection)
{
    // If both are horizontal or both are vertical, they cannot cross
    if((segment1.p2.x - segment1.p1.x == 0 && segment2.p2.x - segment2.p1.x == 0) ||
       (segment1.p2.y - segment1.p1.y == 0 && segment2.p2.y - segment2.p1.y == 0))
    {
        return -1;
    }

    // If segment1 is entirely above or below segment2, they cannot cross
    if((segment1.p1.y > segment2.p1.y && segment1.p2.y > segment2.p1.y &&
        segment1.p1.y > segment2.p2.y && segment1.p2.y > segment2.p2.y) ||
       (segment1.p1.y < segment2.p1.y && segment1.p2.y < segment2.p1.y &&
        segment1.p1.y < segment2.p2.y && segment1.p2.y < segment2.p2.y))
    {
        return -1;
    }

    // If segment1 is entirely to the left or right of segment2, they cannot cross
    if((segment1.p1.x > segment2.p1.x && segment1.p2.x > segment2.p1.x &&
        segment1.p1.x > segment2.p2.x && segment1.p2.x > segment2.p2.x) ||
       (segment1.p1.x < segment2.p1.x && segment1.p2.x < segment2.p1.x &&
        segment1.p1.x < segment2.p2.x && segment1.p2.x < segment2.p2.x))
    {
        return -1;
    }

    // They must cross
    if(segment1.p1.x == segment1.p2.x) // segment1 is vertical
    {
        intersection->x = segment1.p1.x;
        intersection->y = segment2.p1.y;
        return (abs(segment1.p1.x) + abs(segment2.p1.y));
    }
    else // segment1 is horizontal
    {
        intersection->x = segment2.p1.x;
        intersection->y = segment1.p1.y;
        return (abs(segment1.p1.y) + abs(segment2.p1.x));
    }
}

// Returns 0 if successful
// Prints error message and returns -1 if failed
int read_wire_files(char *filename,
                    const int bufsize,
                    Segment_t *wire1, int *wire1_length,
                    Segment_t *wire2, int *wire2_length)
{
    FILE *fp;
    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        return -1;
    }

    *wire1_length = read_wires(bufsize, wire1, fp);
    if(wire1_length < 0)
    {
        printf("Wire 1 read failed with error code %d\n", *wire1_length);
        return -1;
    }

    *wire2_length = read_wires(bufsize, wire2, fp);
    if(wire2_length < 0)
    {
        printf("Wire 2 read failed with error code %d\n", *wire2_length);
        return -1;
    }

    fclose(fp);

    return 0;
}

// Returns -1 if failed
int part1(char *input_filename)
{
    const int bufsize = 500;
    int wire1_length;
    Segment_t wire1[bufsize];
    int wire2_length;
    Segment_t wire2[bufsize];

    if(read_wire_files(input_filename, bufsize, wire1, &wire1_length, wire2, &wire2_length) == -1)
    {
        return -1;
    }

    int min_manhattan_distance = -1;
    Point_t throwaway;
    for(int i = 0; i < wire1_length; ++i)
    {
        for(int j = 0; j < wire2_length; ++j)
        {
            // First segment of each wire, by definition, do not cross
            if(i == 0 && j == 0)
            {
                continue;
            }

            int manhattan_distance = segment_intersection(wire1[i], wire2[j], &throwaway);

            if(manhattan_distance >= 0 &&
              (min_manhattan_distance == -1 || manhattan_distance < min_manhattan_distance))
            {
                min_manhattan_distance = manhattan_distance;
            }
        }
    }

    return min_manhattan_distance;
}

// Returns index of p in buffer_intersections
// Returns -1 if p is not in buffer_intersections
int intersection_index(Point_t p, int num_intersections, Point_t *buffer_intersections)
{
    for(int i = 0; i < num_intersections; ++i)
    {
        if(p.x == buffer_intersections[i].x && p.y == buffer_intersections[i].y)
        {
            return i;
        }
    }

    return -1;
}

// Walks wire, seeing how many steps it takes to get to each intersection
// Returns 0 on success
// Returns -1 on failure
int walk_wire(const int wire_length, Segment_t *wire,
              const int num_intersections, Point_t *buffer_intersections,
              int *step_counts)
{
    int steps = 0;

    Point_t p;
    p.x = 0;
    p.y = 0;

    for(int i = 0; i < wire_length; ++i)
    {
        int dx, dy;
        switch(wire[i].direction)
        {
        case 'U':
            dx = 0;
            dy = 1;
            break;
        case 'D':
            dx = 0;
            dy = -1;
            break;
        case 'L':
            dx = -1;
            dy = 0;
            break;
        case 'R':
            dx = 1;
            dy = 0;
            break;
        default:
            return -1;
        }

        for(int j = 0; j < wire[i].length; ++j)
        {
            steps += 1;
            p.x += dx;
            p.y += dy;
            int index_intersection = intersection_index(p, num_intersections, buffer_intersections);
            if(index_intersection != -1)
            {
                if(step_counts[index_intersection] == 0)
                {
                    step_counts[index_intersection] = steps;
                }
            }
        }
    }

    return 0;
}

// Returns -1 if failed
int part2(char *input_filename)
{
    const int bufsize = 500;
    int wire1_length;
    Segment_t wire1[bufsize];
    int wire2_length;
    Segment_t wire2[bufsize];

    const int bufsize_intersections = 500;
    Point_t buffer_intersections[bufsize_intersections];
    int step_counts1[bufsize_intersections];
    int step_counts2[bufsize_intersections];
    memset(step_counts1, 0, bufsize_intersections * sizeof(int));
    memset(step_counts2, 0, bufsize_intersections * sizeof(int));

    if(read_wire_files(input_filename, bufsize, wire1, &wire1_length, wire2, &wire2_length) == -1)
    {
        return -1;
    }

    // Find all intersection points between the two wires
    int num_intersections = 0;
    for(int i = 0; i < wire1_length; ++i)
    {
        for(int j = 0; j < wire2_length; ++j)
        {
            // First segment of each wire, by definition, do not cross
            if(i == 0 && j == 0)
            {
                continue;
            }

            Point_t intersection;
            if(segment_intersection(wire1[i], wire2[j], &intersection) != -1)
            {
                buffer_intersections[num_intersections].x = intersection.x;
                buffer_intersections[num_intersections].y = intersection.y;
                num_intersections += 1;
                if(num_intersections == bufsize_intersections)
                {
                    return -1;
                }
            }
        }
    }

    // Walk the two wires and find the steps it takes to get to each intersection
    if(walk_wire(wire1_length, wire1, num_intersections, buffer_intersections, step_counts1) == -1 ||
       walk_wire(wire2_length, wire2, num_intersections, buffer_intersections, step_counts2) == -1)
    {
        return -1;
    }

    // Find the fewest combined steps among the wire intersections
    int fewest_combined_steps = step_counts1[0] + step_counts2[0];
    for(int i = 1; i < num_intersections; ++i)
    {
        int combined_steps = step_counts1[i] + step_counts2[i];
        if(combined_steps < fewest_combined_steps)
        {
            fewest_combined_steps = combined_steps;
        }
    }

    return fewest_combined_steps;
}

int main()
{
    char filename[] = "3.txt";
    printf("Part 1: %d\n", part1(filename));
    printf("Part 2: %d\n", part2(filename));
}
