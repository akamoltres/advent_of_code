
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_CHEMICALS 200
#define MAX_CHEMICAL_LENGTH 25
#define MAX_NUM_REACTANTS 10

typedef struct Reaction
{
    int reactant_count;
    char reactants[MAX_NUM_REACTANTS][MAX_CHEMICAL_LENGTH];
    long reactant_qtys[MAX_NUM_REACTANTS];
    char product[MAX_CHEMICAL_LENGTH];
    long product_qty;
} Reaction_t;

// returns the number of reactions
static int get_input(char const *input_filename, Reaction_t reaction_list[MAX_NUM_CHEMICALS - 1])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    int reaction_count = 0;
    int no_more_reactions = 0;

    for (reaction_count = 0; ; ++reaction_count)
    {
        // get reactants
        for (reaction_list[reaction_count].reactant_count = 0; ; ++reaction_list[reaction_count].reactant_count)
        {
            // number of this reactant
            int success = fscanf(fp, "%ld", &reaction_list[reaction_count].reactant_qtys[reaction_list[reaction_count].reactant_count]);

            // end of file
            if (success == EOF)
            {
                no_more_reactions = 1;
                break;
            }

            // nothing matched (it's actually "=>")
            char throwaway[3];
            if (success == 0)
            {
                fscanf(fp, "%s", throwaway);
                break;
            }

            // name of this reactant
            assert(fscanf(fp, "%s", reaction_list[reaction_count].reactants[reaction_list[reaction_count].reactant_count]) == 1);
            int reactant_strlen = strlen(reaction_list[reaction_count].reactants[reaction_list[reaction_count].reactant_count]);
            if (reaction_list[reaction_count].reactants[reaction_list[reaction_count].reactant_count][reactant_strlen - 1] == ',')
            {
                reaction_list[reaction_count].reactants[reaction_list[reaction_count].reactant_count][reactant_strlen - 1] = '\0';
            }

            // memory allocation check
            assert(reaction_list[reaction_count].reactant_count < MAX_NUM_REACTANTS);
        }

        // end of file
        if (no_more_reactions)
        {
            break;
        }

        // get product
        assert(fscanf(fp, "%ld %s\n", &reaction_list[reaction_count].product_qty, reaction_list[reaction_count].product) == 2);

        // memory allocation check (reserve the last spot for ORE)
        assert(reaction_count < MAX_NUM_CHEMICALS - 1);
    }

    fclose(fp);

    return reaction_count;
}

static int get_chemical_index(const char *chemical, int reaction_count, Reaction_t reaction_list[MAX_NUM_CHEMICALS - 1])
{
    if (!strcmp(chemical, "ORE"))
    {
        return reaction_count;
    }

    for (int i = 0; i < reaction_count; ++i)
    {
        if (!strcmp(chemical, reaction_list[i].product))
        {
            return i;
        }
    }

    assert(0);
}

static void count_ore_required(const char *chemical, long num_required, int reaction_count, long chemical_stock[MAX_NUM_CHEMICALS], Reaction_t reaction_list[MAX_NUM_CHEMICALS - 1])
{
    if (!strcmp(chemical, "ORE"))
    {
        chemical_stock[reaction_count] += num_required;
        return ;
    }

    int product_index = get_chemical_index(chemical, reaction_count, reaction_list);
    chemical_stock[product_index] -= num_required;

    // generate more if needed
    while (chemical_stock[product_index] < 0)
    {
        chemical_stock[product_index] += reaction_list[product_index].product_qty;
        for (int i = 0; i < reaction_list[product_index].reactant_count; ++i)
        {
            count_ore_required(reaction_list[product_index].reactants[i], reaction_list[product_index].reactant_qtys[i], reaction_count, chemical_stock, reaction_list);
        }
    }
}

long solve_2019_14_1(char const *input_filename)
{
    // ORE is not created by a reaction
    Reaction_t reaction_list[MAX_NUM_CHEMICALS - 1];
    memset(reaction_list, 0, (MAX_NUM_CHEMICALS - 1) * sizeof(Reaction_t));

    int reaction_count = get_input(input_filename, reaction_list);

    long chemical_stock[MAX_NUM_CHEMICALS] = {0};
    count_ore_required("FUEL", 1, reaction_count, chemical_stock, reaction_list);

    return chemical_stock[reaction_count];
}

long solve_2019_14_2(char const *input_filename)
{
    return -1;
}
