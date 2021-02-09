
#include "util.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// there are four moons per the problem spec
#define NUM_MOONS 4
#define AXES 3

typedef struct Moon
{
    long pos[3];
    long vel[3];
} Moon_t;

void init_moons(char const *filename, Moon_t moons[NUM_MOONS])
{
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);

    for (int i = 0; i < NUM_MOONS; ++i)
    {
        fscanf(fp, "<x=%ld, y=%ld, z=%ld>\n", &moons[i].pos[0], &moons[i].pos[1], &moons[i].pos[2]);
        moons[i].vel[0] = moons[i].vel[1] = moons[i].vel[2] = 0;
    }

    fclose(fp);
}

void step_moons_axis(Moon_t moons[NUM_MOONS], int axis)
{
    assert(0 <= axis && axis < AXES);

    // update velocities
    for (int m = 0; m < NUM_MOONS; ++m)
    {
        for (int i = 0; i < NUM_MOONS; ++i)
        {
            if (i != m)
            {
                if (moons[m].pos[axis] > moons[i].pos[axis])
                {
                    moons[m].vel[axis] -= 1;
                }
                else if (moons[m].pos[axis] < moons[i].pos[axis])
                {
                    moons[m].vel[axis] += 1;
                }
            }
        }
    }

    // update positions
    for (int m = 0; m < NUM_MOONS; ++m)
    {
        moons[m].pos[axis] += moons[m].vel[axis];
    }
}

void step_moons(Moon_t moons[NUM_MOONS])
{
    for (int i = 0; i < AXES; ++i)
    {
        step_moons_axis(moons, i);
    }
}

long get_energy(long *coords) { return abs(*coords) + abs(*(coords + 1)) + abs(*(coords + 2)); }

long get_total_energy(Moon_t *moon) { return get_energy(moon->pos) * get_energy(moon->vel); }

long get_all_energy(Moon_t moons[NUM_MOONS])
{
    long total_energy = 0;
    for (int m = 0; m < NUM_MOONS; ++m)
    {
        total_energy += get_total_energy(&moons[m]);
    }
    return total_energy;
}

long solve_2019_12_1(char const *input_filename, int num_steps)
{
    Moon_t moons[NUM_MOONS];
    init_moons(input_filename, moons);

    for (int s = 0; s < num_steps; ++s)
    {
        step_moons(moons);
    }

    return get_all_energy(moons);
}

char moons_axis_equal(Moon_t moons1[NUM_MOONS], Moon_t moons2[NUM_MOONS], int axis)
{
    for (int i = 0; i < NUM_MOONS; ++i)
    {
        if (moons1[i].pos[axis] != moons2[i].pos[axis] ||
            moons1[i].vel[axis] != moons2[i].vel[axis])
        {
            return 0;
        }
    }
    return 1;
}

long solve_2019_12_2(char const *input_filename)
{
    Moon_t moons[NUM_MOONS];
    init_moons(input_filename, moons);

    // copy the initial state
    Moon_t moons_start[NUM_MOONS];
    memcpy(moons_start, moons, NUM_MOONS * sizeof(Moon_t));

    // independently handle x, y, and z axes
    long steps[AXES];
    for (int a = 0; a < AXES; ++a)
    {
        for (steps[a] = 1;; ++steps[a])
        {
            step_moons_axis(moons, a);

            if (moons_axis_equal(moons, moons_start, a))
            {
                break;
            }
        }
    }

    return lcm(steps[0], lcm(steps[1], steps[2]));
}
