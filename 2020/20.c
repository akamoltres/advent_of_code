
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define TILE_DIM 10
#define MAX_SIDE_LEN 15
#define MAX_NUM_TILES MAX_SIDE_LEN * MAX_SIDE_LEN

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

// returns 1 if the tile is a corner, 0 otherwise
int is_corner(int idx, int num_tiles, Tile_t tiles[MAX_NUM_TILES], TileSides_t sides[MAX_NUM_TILES])
{
    int matched_edges = 0;

    for(int i = 0; i < 8; i += 2)
    {
        int matched = 0;

        for(int j = 0; !matched && j < num_tiles; ++j)
        {
            if(tiles[idx].id != tiles[j].id)
            {
                for(int l = 0; !matched && l < 8; ++l)
                {
                    matched = (!strcmp(sides[idx].s[i],     sides[j].s[l]) ||
                               !strcmp(sides[idx].s[i + 1], sides[j].s[l]));
                }
            }
        }

        matched_edges += matched;
    }

    return (matched_edges == 2);
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
        if(is_corner(i, num_tiles, tiles, sides))
        {
            retval *= tiles[i].id;
        }
    }

    return retval;
}

void rot90(Tile_t *tile)
{
    Tile_t rot;
    memset(rot.p, 0, TILE_DIM * (TILE_DIM + 1) * sizeof(char));

    for(int i = 0; i < TILE_DIM; ++i)
    {
        for(int j = 0; j < TILE_DIM; ++j)
        {
            rot.p[i][j] = tile->p[TILE_DIM - j - 1][i];
        }
    }

    memcpy(tile->p, rot.p, TILE_DIM * (TILE_DIM + 1) * sizeof(char));
}

// side 1 is top
// side 2 is right
// side 3 is bottom
// side 4 is left
void get_side(Tile_t *tile, int side, char s[TILE_DIM + 1])
{
    switch(side)
    {
        case 1:
            strcpy(s, tile->p[0]);
            break;
        case 2:
            for(int i = 0; i < TILE_DIM; ++i)
            {
                s[i] = tile->p[i][TILE_DIM - 1];
            }
            s[TILE_DIM] = '\0';
        case 3:
            strcpy(s, tile->p[TILE_DIM - 1]);
            break;
        case 4:
            for(int i = 0; i < TILE_DIM; ++i)
            {
                s[i] = tile->p[i][0];
            }
            s[TILE_DIM] = '\0';
            break;
        default:
            assert(0);
    }
}

/*
static void print_tile(Tile_t t)
{
    printf("%lu\n", t.id);
    for(int i = 0; i < TILE_DIM; ++i)
    {
        printf("%s\n", t.p[i]);
    }
}
*/

// side 1 is top
// side 2 is right
// side 3 is bottom
// side 4 is left
// returns 0 if they match, something else if they don't
int compare_sides(Tile_t *tile1, int side1, Tile_t *tile2, int side2)
{
    char t1s[TILE_DIM + 1];
    char t2s[TILE_DIM + 1];

    get_side(tile1, side1, t1s);
    get_side(tile2, side2, t2s);

    printf("%s %s\n", t1s, t2s);

    return strcmp(t1s, t2s);
}

int side_has_match(int tile_idx, int side, int num_tiles, Tile_t tiles[MAX_NUM_TILES])
{
}

void assemble_grid(int num_tiles, int side_length,
                   Tile_t tiles[MAX_NUM_TILES],
                   Tile_t grid[MAX_SIDE_LEN][MAX_SIDE_LEN])
{
    // int tile_used[MAX_NUM_TILES] = {0};

    // step 0: generate strings for all the edges
    TileSides_t sides[MAX_NUM_TILES];
    generate_sides(num_tiles, tiles, sides);

    // step 1: find a corner tile
    int first_corner = -1;
    for(int i = 0; i < num_tiles; ++i)
    {
        if(is_corner(i, num_tiles, tiles, sides))
        {
            first_corner = i;
            break;
        }
    }
    assert(first_corner != -1);
    printf("%lu\n", tiles[first_corner].id);

    // step 2: place the corner tile

    // step 3: walk through the grid, placing tiles as they are found to fit
}

int part2(char *input_filename)
{
    Tile_t tiles[MAX_NUM_TILES];

    int num_tiles = get_inputs(input_filename, tiles);

    Tile_t grid[MAX_SIDE_LEN][MAX_SIDE_LEN];
    int side_length = (int) (sqrt((double) num_tiles) + 0.5);

    assemble_grid(num_tiles, side_length, tiles, grid);

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
