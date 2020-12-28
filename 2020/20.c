
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define TILE_DIM 10
#define MAX_SIDE_LEN 15
#define MAX_NUM_TILES MAX_SIDE_LEN * MAX_SIDE_LEN
#define MAX_SIDE_PX MAX_SIDE_LEN * (TILE_DIM - 2)
#define WIDTH_MONSTER_PX 20
#define HEIGHT_MONSTER_PX 3

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
            break;
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

void generate_sides(int num_tiles, Tile_t tiles[MAX_NUM_TILES], TileSides_t sides[MAX_NUM_TILES])
{
    for(int i = 0; i < num_tiles; ++i)
    {
        // top
        get_side(&tiles[i], 1, sides[i].s[0]);
        strcpy_flip(sides[i].s[1], sides[i].s[0]);

        // right
        get_side(&tiles[i], 2, sides[i].s[6]);
        strcpy_flip(sides[i].s[7], sides[i].s[6]);

        // bottom
        get_side(&tiles[i], 3, sides[i].s[2]);
        strcpy_flip(sides[i].s[3], sides[i].s[2]);

        // left
        get_side(&tiles[i], 4, sides[i].s[4]);
        strcpy_flip(sides[i].s[5], sides[i].s[4]);
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

void flip(Tile_t *tile)
{
    Tile_t flip;
    memset(flip.p, 0, TILE_DIM * (TILE_DIM + 1) * sizeof(char));
    
    for(int i = 0; i < TILE_DIM; ++i)
    {
        strcpy_flip(flip.p[i], tile->p[i]);
    }

    memcpy(tile->p, flip.p, TILE_DIM * (TILE_DIM + 1) * sizeof(char));
}

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

    return strcmp(t1s, t2s);
}

// check if this side matches with the side of any other tile
int side_has_match(int tile_idx, int side, int num_tiles, Tile_t tiles[MAX_NUM_TILES])
{
    char candidate_side[TILE_DIM + 1];
    get_side(&tiles[tile_idx], side, candidate_side);

    for(int i = 0; i < num_tiles; ++i)
    {
        if(i != tile_idx)
        {
            for(int j = 1; j <= 4; ++j)
            {
                char test_side[TILE_DIM + 1];

                get_side(&tiles[i], j, test_side);
                if(!strcmp(candidate_side, test_side))
                {
                    return 1;
                }

                char test_side_flipped[TILE_DIM + 1];
                strcpy_flip(test_side_flipped, test_side);
                if(!strcmp(candidate_side, test_side_flipped))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void assemble_grid(int num_tiles, int side_length,
                   Tile_t tiles[MAX_NUM_TILES],
                   Tile_t grid[MAX_SIDE_LEN][MAX_SIDE_LEN])
{
    int tile_used[MAX_NUM_TILES] = {0};

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

    // step 2: orient and place the corner tile
    int first_corner_oriented = 0;
    for(int i = 0; i < 4; ++i)
    {
        if(side_has_match(first_corner, 1, num_tiles, tiles) || side_has_match(first_corner, 4, num_tiles, tiles))
        {
            rot90(&tiles[first_corner]);
        }
        else
        {
            first_corner_oriented = 1;
            assert(!side_has_match(first_corner, 1, num_tiles, tiles));
            assert(!side_has_match(first_corner, 4, num_tiles, tiles));
            assert(side_has_match(first_corner, 2, num_tiles, tiles));
            assert(side_has_match(first_corner, 3, num_tiles, tiles));
            break;
        }
    }
    assert(first_corner_oriented);
    tile_used[first_corner] = 1;
    memcpy(&grid[0][0], &tiles[first_corner], sizeof(Tile_t));

    // step 3: walk through the grid, placing tiles as they are found to fit
    for(int r = 0; r < side_length; ++r)
    {
        for(int c = 0; c < side_length; ++c)
        {
            // first corner is already placed
            if(r == 0 && c == 0)
            {
                continue;
            }

            // try all orientations of all tiles until one fits
            int tile_placed = 0;
            for(int i = 0; !tile_placed && i < num_tiles; ++i)
            {
                if(!tile_used[i])
                {
                    for(int j = 0; !tile_placed && j < 2; ++j)
                    {
                        for(int k = 0; !tile_placed && k < 4; ++k)
                        {
                            int top_good = (r == 0);
                            int left_good = (c == 0);

                            if(!top_good)
                            {
                                char top[TILE_DIM + 1] = {0};
                                get_side(&tiles[i], 1, top);

                                char bottom[TILE_DIM + 1] = {0};
                                get_side(&grid[r - 1][c], 3, bottom);
                                top_good = (strcmp(top, bottom) == 0);
                            }

                            if(!left_good)
                            {
                                char left[TILE_DIM + 1] = {0};
                                get_side(&tiles[i], 4, left);

                                char right[TILE_DIM + 1] = {0};
                                get_side(&grid[r][c - 1], 2, right);

                                left_good = (strcmp(left, right) == 0);
                            }

                            tile_placed = (top_good && left_good);

                            if(!tile_placed)
                            {
                                rot90(&tiles[i]);
                            }
                            else
                            {
                                tile_used[i] = 1;
                                memcpy(&grid[r][c], &tiles[i], sizeof(Tile_t));
                            }
                        }
                        
                        if(!tile_placed)
                        {
                            flip(&tiles[i]);
                        }
                    }
                }
            }
            assert(tile_placed);
        }
    }

    // step 4: verify all the tiles are used
    for(int i = 0; i < num_tiles; ++i)
    {
        assert(tile_used[i]);
    }
}

void rot90_img(int sidelen, char img[MAX_SIDE_PX][MAX_SIDE_PX + 1])
{
    char buffer[MAX_SIDE_PX][MAX_SIDE_PX + 1];
    memset(buffer, 0, MAX_SIDE_PX * (MAX_SIDE_PX + 1) * sizeof(char));

    for(int i = 0; i < sidelen; ++i)
    {
        for(int j = 0; j < sidelen; ++j)
        {
            buffer[i][j] = img[sidelen - j - 1][i];
        }
    }

    memcpy(img, buffer, MAX_SIDE_PX * (MAX_SIDE_PX + 1) * sizeof(char));
}

void flip_img(int sidelen, char img[MAX_SIDE_PX][MAX_SIDE_PX + 1])
{
    char buffer[MAX_SIDE_PX][MAX_SIDE_PX + 1];
    memset(buffer, 0, MAX_SIDE_PX * (MAX_SIDE_PX + 1) * sizeof(char));

    for(int i = 0; i < sidelen; ++i)
    {
        for(int j = 0; j < sidelen; ++j)
        {
            strcpy_flip(buffer[i], img[i]);
        }
    }

    memcpy(img, buffer, MAX_SIDE_PX * (MAX_SIDE_PX + 1) * sizeof(char));
}

// counts sea monsters, replacing them with 'O'
int count_sea_monsters(int side_length, char image[MAX_SIDE_PX][MAX_SIDE_PX + 1])
{
    int count = 0;

    for(int i = 0; i + HEIGHT_MONSTER_PX < side_length; ++i)
    {
        for(int j = 0; j + WIDTH_MONSTER_PX < side_length; ++j)
        {
            if(image[i][j + 18] == '#' &&
               image[i + 1][j] == '#' &&
               image[i + 1][j + 5] == '#' &&
               image[i + 1][j + 6] == '#' &&
               image[i + 1][j + 11] == '#' &&
               image[i + 1][j + 12] == '#' &&
               image[i + 1][j + 17] == '#' &&
               image[i + 1][j + 18] == '#' &&
               image[i + 1][j + 19] == '#' &&
               image[i + 2][j + 1] == '#' &&
               image[i + 2][j + 4] == '#' &&
               image[i + 2][j + 7] == '#' &&
               image[i + 2][j + 10] == '#' &&
               image[i + 2][j + 13] == '#' &&
               image[i + 2][j + 16] == '#')
            {
                count += 1;
                image[i][j + 18] = 'O';
                image[i + 1][j] = 'O';
                image[i + 1][j + 5] = 'O';
                image[i + 1][j + 6] = 'O';
                image[i + 1][j + 11] = 'O';
                image[i + 1][j + 12] = 'O';
                image[i + 1][j + 17] = 'O';
                image[i + 1][j + 18] = 'O';
                image[i + 1][j + 19] = 'O';
                image[i + 2][j + 1] = 'O';
                image[i + 2][j + 4] = 'O';
                image[i + 2][j + 7] = 'O';
                image[i + 2][j + 10] = 'O';
                image[i + 2][j + 13] = 'O';
                image[i + 2][j + 16] = 'O';
            }
        }
    }

    return count;
}

int part2(char *input_filename)
{
    Tile_t tiles[MAX_NUM_TILES];

    int num_tiles = get_inputs(input_filename, tiles);

    Tile_t grid[MAX_SIDE_LEN][MAX_SIDE_LEN];
    int side_length = (int) (sqrt((double) num_tiles) + 0.5);

    assemble_grid(num_tiles, side_length, tiles, grid);

    // build the image
    char image[MAX_SIDE_PX][MAX_SIDE_PX + 1];
    memset(image, 0, MAX_SIDE_PX * (MAX_SIDE_PX + 1) * sizeof(char));
    for(int r = 0; r < side_length; ++r)
    {
        for(int c = 0; c < side_length; ++c)
        {
            for(int k = 1; k + 1 < TILE_DIM; ++k)
            {
                for(int l = 1; l + 1 < TILE_DIM; ++l)
                {
                    image[r * (TILE_DIM - 2) + k - 1][c * (TILE_DIM - 2) + l - 1] = grid[r][c].p[k][l];
                }
            }
        }
    }

    // orient and find sea monsters
    int num_sea_monsters = 0;
    for(int i = 0; num_sea_monsters == 0 && i < 2; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            num_sea_monsters = count_sea_monsters(side_length * (TILE_DIM - 2), image);

            if(num_sea_monsters == 0)
            {
                rot90_img(side_length * (TILE_DIM - 2), image);
            }
            else
            {
                break;
            }
        }

        if(num_sea_monsters == 0)
        {
            flip_img(side_length * (TILE_DIM - 2), image);
        }
    }

    // count the roughness
    int roughness = 0;
    for(int i = 0; i < side_length * (TILE_DIM - 2); ++i)
    {
        for(int j = 0; j < side_length * (TILE_DIM - 2); ++j)
        {
            roughness += (image[i][j] == '#');
        }
    }

    return roughness;
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
