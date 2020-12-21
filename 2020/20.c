
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TILE_DIM 10
#define MAX_NUM_TILES 200

typedef unsigned long ul;

typedef struct Tile
{
    ul id;
    char p[TILE_DIM][TILE_DIM + 1];
} Tile_t;

typedef struct TileSides
{
    char s[8][TILE_DIM + 1];
} TileSides_t;

int get_inputs(char *input_filename, Tile_t tiles[MAX_NUM_TILES])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    int num_tiles = 0;

    while(fscanf(fp, "Tile %lu:\n", &tiles[num_tiles].id) != EOF)
    {
        char c[2];

        for(int i = 0; i < TILE_DIM; ++i)
        {
            fgets(tiles[num_tiles].p[i], TILE_DIM + 1, fp);
            fgets(c, 2, fp); // throw away the newline
        }

        // throw away the empty line
        fgets(c, 2, fp);

        num_tiles += 1;
        assert(num_tiles != MAX_NUM_TILES);
    }

    fclose(fp);

    return num_tiles;
}

void strcpy_flip(char *dest, char *src)
{
    int len = strlen(src);
    for(int i = 0; i < len; ++i)
    {
        dest[i] = src[len - i - 1];
    }
    dest[len] = '\0';
}

void generate_sides(int num_tiles, Tile_t tiles[MAX_NUM_TILES], TileSides_t sides[MAX_NUM_TILES])
{
    for(int i = 0; i < num_tiles; ++i)
    {
        // top
        strcpy(sides[i].s[0], tiles[i].p[0]);
        strcpy_flip(sides[i].s[1], sides[i].s[0]);

        // bottom
        strcpy(sides[i].s[2], tiles[i].p[TILE_DIM - 1]);
        strcpy_flip(sides[i].s[3], sides[i].s[2]);

        // left
        for(int j = 0; j < TILE_DIM; ++j)
        {
            sides[i].s[4][j] = tiles[i].p[j][0];
        }
        sides[i].s[4][TILE_DIM] = '\0';
        strcpy_flip(sides[i].s[5], sides[i].s[4]);

        // right
        for(int j = 0; j < TILE_DIM; ++j)
        {
            sides[i].s[6][j] = tiles[i].p[j][TILE_DIM - 1];
        }
        sides[i].s[6][TILE_DIM] = '\0';
        strcpy_flip(sides[i].s[7], sides[i].s[6]);
    }
}

ul part1(char *input_filename)
{
    Tile_t tiles[MAX_NUM_TILES];
    TileSides_t sides[MAX_NUM_TILES];

    int num_tiles = get_inputs(input_filename, tiles);
    generate_sides(num_tiles, tiles, sides);

    ul retval = 1;

    for(int i = 0; i < num_tiles; ++i)
    {
        int unmatched_edges = 0;

        for(int j = 0; j < 8; j += 2)
        {
            int matched = 0;
            for(int k = 0; !matched && k < num_tiles; ++k)
            {
                if(i != k)
                {
                    for(int l = 0; !matched && l < 8; ++l)
                    {
                        matched = (!strcmp(sides[i].s[j], sides[k].s[l]) || !strcmp(sides[i].s[j + 1], sides[k].s[l]));
                    }
                }
            }
            unmatched_edges += matched;
        }

        if(unmatched_edges == 2)
        {
            retval *= tiles[i].id;
        }
    }

    return retval;
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

    printf("Part 1: %lu\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
