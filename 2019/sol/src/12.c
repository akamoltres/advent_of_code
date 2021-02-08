
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// there are four moons per the problem spec
#define NUM_MOONS 4

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

long get_energy(long *coords) { return abs(*coords) + abs(*(coords + 1)) + abs(*(coords + 2)); }

long get_total_energy(Moon_t *moon) { return get_energy(moon->pos) * get_energy(moon->vel); }

long solve_2019_12_1(char const *input_filename, int num_steps)
{
    Moon_t moons[NUM_MOONS];
    init_moons(input_filename, moons);

    for (int s = 0; s < num_steps; ++s)
    {
        // update velocities
        for (int m = 0; m < NUM_MOONS; ++m)
        {
            for (int i = 0; i < NUM_MOONS; ++i)
            {
                if (i != m)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        if (moons[m].pos[j] > moons[i].pos[j])
                        {
                            moons[m].vel[j] -= 1;
                        }
                        else if (moons[m].pos[j] < moons[i].pos[j])
                        {
                            moons[m].vel[j] += 1;
                        }
                    }
                }
            }
        }

        // update positions
        for (int m = 0; m < NUM_MOONS; ++m)
        {
            for (int i = 0; i < 3; ++i)
            {
                moons[m].pos[i] += moons[m].vel[i];
            }
        }
    }

    // calculate total energy
    long total_energy = 0;
    for (int m = 0; m < NUM_MOONS; ++m)
    {
        total_energy += get_total_energy(&moons[m]);
    }

    return total_energy;
}
