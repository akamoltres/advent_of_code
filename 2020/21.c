
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_FOODS 100
#define MAX_NUM_ITEM 500
#define MAX_ITEM_LEN 15

typedef struct Food
{
    int num_ingr;
    char ingr[MAX_NUM_ITEM][MAX_ITEM_LEN];
    int num_aller;
    char aller[MAX_NUM_ITEM][MAX_ITEM_LEN];
} Food_t;

int get_input(char *input_filename, Food_t foods[MAX_NUM_FOODS])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    int num_foods = 0;

    while(1)
    {
        // get the ingredients
        foods[num_foods].num_ingr = -1;
        int *num_ingr = &foods[num_foods].num_ingr;
        do {
            if(fscanf(fp, "%s", foods[num_foods].ingr[++*num_ingr]) == EOF)
            {
                fclose(fp);
                return num_foods;
            }
            assert(*num_ingr != MAX_NUM_ITEM);
        } while(strcmp(foods[num_foods].ingr[*num_ingr], "(contains"));

        // get the allergens
        foods[num_foods].num_aller = 0;
        int *num_aller = &foods[num_foods].num_aller;
        do {
            fscanf(fp, "%s", foods[num_foods].aller[(*num_aller)++]);
            assert(*num_aller != MAX_NUM_ITEM);
        } while(foods[num_foods].aller[*num_aller - 1][strlen(foods[num_foods].aller[*num_aller - 1]) - 1] != ')');

        // strip the comma/parenthesis from the allergens
        for(int i = 0; i < *num_aller; ++i)
        {
            foods[num_foods].aller[i][strlen(foods[num_foods].aller[i]) - 1] = '\0';
        }

        num_foods += 1;
    }

    assert(0);
}

int add_item_if_missing(char list[MAX_NUM_ITEM][MAX_ITEM_LEN], int *current_count, char *new_item)
{
    for(int i = 0; i < *current_count; ++i)
    {
        if(!strcmp(list[i], new_item))
        {
            return *current_count;
        }
    }

    strcpy(list[(*current_count)++], new_item);

    return (*current_count);
}

int get_ingr_idx(char ingrs[MAX_NUM_ITEM][MAX_ITEM_LEN], int num_ingrs, char *ingr)
{
    for(int i = 0; i < num_ingrs; ++i)
    {
        if(!strcmp(ingrs[i], ingr))
        {
            return i;
        }
    }
    assert(0);
}

int ingr_is_matched(int aller_src[MAX_NUM_ITEM], int num_allers, int ingr_idx)
{
    for(int i = 0; i < num_allers; ++i)
    {
        if(aller_src[i] == ingr_idx)
        {
            return 1;
        }
    }
    return 0;
}

int part1(char *input_filename)
{
    // init
    int num_foods = 0;
    Food_t foods[MAX_NUM_FOODS];

    int num_ingrs = 0;
    char ingrs[MAX_NUM_ITEM][MAX_ITEM_LEN];
    memset(ingrs, 0, MAX_NUM_ITEM * MAX_ITEM_LEN * sizeof(char));

    int num_allers = 0;
    char allers[MAX_NUM_ITEM][MAX_ITEM_LEN];
    memset(allers, 0, MAX_NUM_ITEM * MAX_ITEM_LEN * sizeof(char));

    int aller_src[MAX_NUM_ITEM];
    for(int i = 0; i < MAX_NUM_ITEM; ++i)
    {
        aller_src[i] = -1;
    }

    // input
    num_foods = get_input(input_filename, foods);

    // build lists of ingredients and allergens
    for(int i = 0; i < num_foods; ++i)
    {
        for(int j = 0; j < foods[i].num_ingr; ++j)
        {
            assert(add_item_if_missing(ingrs, &num_ingrs, foods[i].ingr[j]) < MAX_NUM_ITEM);
        }

        for(int j = 0; j < foods[i].num_aller; ++j)
        {
            assert(add_item_if_missing(allers, &num_allers, foods[i].aller[j]) < MAX_NUM_ITEM);
        }
    }

    // match each allergen to an ingredient
    for(int try = 0; try < num_allers; ++try)
    {
        for(int i = 0; i < num_allers; ++i)
        {
            int candidates[MAX_NUM_ITEM];
            memset(candidates, 0, MAX_NUM_ITEM * sizeof(int));

            int first = 1;

            // walk through the foods
            for(int j = 0; j < num_foods; ++j)
            {
                // walk through the allergens in the food
                for(int k = 0; k < foods[j].num_aller; ++k)
                {
                    // if the allergen is the one we are currently looking at
                    if(!strcmp(allers[i], foods[j].aller[k]))
                    {
                        // if it is the first instance, we have our initial list of candidates
                        if(first)
                        {
                            first = 0;
                            for(int l = 0; l < foods[j].num_ingr; ++l)
                            {
                                int ingr_idx = get_ingr_idx(ingrs, num_ingrs, foods[j].ingr[l]);
                                if(!ingr_is_matched(aller_src, num_allers, ingr_idx))
                                {
                                    candidates[ingr_idx] = 1;
                                }
                            }
                        }
                        // if it is not the first instance, we can whittle down the list of candidates
                        else
                        {
                            for(int l = 0; l < foods[j].num_ingr; ++l)
                            {
                                int ingr_idx = get_ingr_idx(ingrs, num_ingrs, foods[j].ingr[l]);
                                if(candidates[ingr_idx])
                                {
                                    candidates[ingr_idx] = 2;
                                }
                            }
                            for(int l = 0; l < num_ingrs; ++l)
                            {
                                if(candidates[l] == 2)
                                {
                                    candidates[l] = 1;
                                }
                                else if(candidates[l] == 1)
                                {
                                    candidates[l] = 0;
                                }
                            }
                        }
                    }
                }
            }

            // there should be one candidate remaining
            int candidate = -1;
            for(int j = 0; j < num_ingrs; ++j)
            {
                if(candidates[j] == 1)
                {
                    if(candidate == -1)
                    {
                        candidate = j;
                    }
                    else
                    {
                        candidate = -2;
                    }
                }
            }
            if(candidate >= 0)
            {
                aller_src[i] = candidate;
            }
        }
    }

    // count the hypoallergenic ingredients
    int hypo_count = 0;
    for(int i = 0; i < num_foods; ++i)
    {
        for(int j = 0; j < foods[i].num_ingr; ++j)
        {
            hypo_count += !ingr_is_matched(aller_src, num_allers, get_ingr_idx(ingrs, num_ingrs, foods[i].ingr[j]));
        }
    }

    return hypo_count;
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

    printf("Part 1: %d\n", part1(argv[1]));
    printf("Part 2: %d\n", part2(argv[1]));

    return 0;
}
