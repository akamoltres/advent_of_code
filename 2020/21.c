
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_FOODS 100
#define MAX_NUM_ITEM 500
#define MAX_ITEM_LEN 15
#define MAX_RETVAL_LEN 100

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

int get_item_idx(char items[MAX_NUM_ITEM][MAX_ITEM_LEN], int num_items, char *item)
{
    for(int i = 0; i < num_items; ++i)
    {
        if(!strcmp(items[i], item))
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

void build_lists_from_foods(int num_foods, Food_t foods[MAX_NUM_FOODS],
                            int *num_ingrs, char ingrs[MAX_NUM_ITEM][MAX_ITEM_LEN],
                            int *num_allers, char allers[MAX_NUM_ITEM][MAX_ITEM_LEN])
{
    for(int i = 0; i < num_foods; ++i)
    {
        for(int j = 0; j < foods[i].num_ingr; ++j)
        {
            assert(add_item_if_missing(ingrs, num_ingrs, foods[i].ingr[j]) < MAX_NUM_ITEM);
        }

        for(int j = 0; j < foods[i].num_aller; ++j)
        {
            assert(add_item_if_missing(allers, num_allers, foods[i].aller[j]) < MAX_NUM_ITEM);
        }
    }
}

int match_allers_to_ingrs(int num_foods, Food_t foods[MAX_NUM_FOODS],
                          int num_allers, char allers[MAX_NUM_ITEM][MAX_ITEM_LEN],
                          int num_ingrs, char ingrs[MAX_NUM_ITEM][MAX_ITEM_LEN],
                          int aller_src[MAX_NUM_ITEM])
{
    int allers_matched = 0;

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
                                int ingr_idx = get_item_idx(ingrs, num_ingrs, foods[j].ingr[l]);
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
                                int ingr_idx = get_item_idx(ingrs, num_ingrs, foods[j].ingr[l]);
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
                allers_matched += 1;
            }
        }
    }

    return allers_matched;
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
    build_lists_from_foods(num_foods, foods, &num_ingrs, ingrs, &num_allers, allers);

    // match each allergen to an ingredient
    assert(match_allers_to_ingrs(num_foods, foods,
                          num_allers, allers,
                          num_ingrs, ingrs,
                          aller_src) == num_allers);

    // count the instances of hypoallergenic ingredients
    int hypo_count = 0;
    for(int i = 0; i < num_foods; ++i)
    {
        for(int j = 0; j < foods[i].num_ingr; ++j)
        {
            hypo_count += !ingr_is_matched(aller_src, num_allers, get_item_idx(ingrs, num_ingrs, foods[i].ingr[j]));
        }
    }

    return hypo_count;
}

void part2(char *input_filename, char retval[MAX_RETVAL_LEN])
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
    build_lists_from_foods(num_foods, foods, &num_ingrs, ingrs, &num_allers, allers);

    // match each allergen to an ingredient
    assert(match_allers_to_ingrs(num_foods, foods,
                          num_allers, allers,
                          num_ingrs, ingrs,
                          aller_src) == num_allers);

    // bubble sort time
    char allers_sorted[MAX_NUM_ITEM][MAX_ITEM_LEN];
    memcpy(allers_sorted, allers, MAX_NUM_ITEM * MAX_ITEM_LEN * sizeof(char));
    for(int i = 0; i < num_allers; ++i)
    {
        for(int j = 1; j < num_allers; ++j)
        {
            if(strcmp(allers_sorted[j - 1], allers_sorted[j]) > 0)
            {
                char tmp[MAX_ITEM_LEN];
                strcpy(tmp, allers_sorted[j - 1]);
                strcpy(allers_sorted[j - 1], allers_sorted[j]);
                strcpy(allers_sorted[j], tmp);
            }
        }
    }

    // return the sorted results
    memset(retval, 0, MAX_RETVAL_LEN * sizeof(char));
    int offset = 0;
    for(int i = 0; i < num_allers; ++i)
    {
        char *ingr = ingrs[aller_src[get_item_idx(allers, num_allers, allers_sorted[i])]];
        assert(strlen(ingr) + 1 + offset < MAX_RETVAL_LEN);
        strcpy(retval + offset, ingr);
        offset += strlen(ingr);
        if(i != num_allers - 1)
        {
            retval[offset++] = ',';
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Exactly 1 argument (input file) required\n");
        return -1;
    }

    printf("Part 1: %d\n", part1(argv[1]));

    char canonical_list[MAX_RETVAL_LEN];
    part2(argv[1], canonical_list);
    printf("Part 2: %s\n", canonical_list);

    return 0;
}
