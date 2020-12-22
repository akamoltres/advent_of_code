
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 500
#define MAX_NUM_PATTERNS 5
#define MAX_PATTERN_LENGTH MAX_NUM_PATTERNS
#define MAX_MSG MAX_RULES
#define MAX_STR_LEN 100

typedef struct Rule
{
    int id;
    int type; // 0 for rule, 1 for char
    int pattern[MAX_NUM_PATTERNS][MAX_PATTERN_LENGTH];
    int num_patterns;
    int pattern_size[MAX_NUM_PATTERNS];
    char c;
} Rule_t;

// returns the index of rule zero
void get_input(char *input_filename,
               int *num_rules, Rule_t rules[MAX_RULES],
               int *num_messages, char messages[MAX_MSG][MAX_STR_LEN])
{
    FILE *fp = fopen(input_filename, "r");
    assert(fp != NULL);

    char c[2];

    // get the rules
    *num_rules = 0;
    while(1)
    {
        assert(*num_rules < MAX_RULES);

        int not_done = fscanf(fp, "%d", &rules[*num_rules].id);
        if(!not_done)
        {
            break;
        }

        // throw away ':'
        fgets(c, 2, fp);

        char buffer[MAX_STR_LEN];
        assert(fgets(buffer, MAX_STR_LEN, fp) != NULL);
        char *token = strtok(buffer, " ");
        rules[*num_rules].num_patterns = 0;
        rules[*num_rules].pattern_size[0] = 0;
        rules[*num_rules].type = 0;
        while(token != NULL)
        {
            if(token[0] == '\"')
            {
                rules[*num_rules].type = 1;
                rules[*num_rules].c = token[1];
                break;
            }

            assert(rules[*num_rules].pattern_size[rules[*num_rules].num_patterns] < MAX_PATTERN_LENGTH);
            if(token[0] == '|')
            {
                assert(rules[*num_rules].num_patterns < MAX_NUM_PATTERNS);
                rules[*num_rules].num_patterns += 1;
                rules[*num_rules].pattern_size[rules[*num_rules].num_patterns] = 0;
            }
            else
            {
                assert(rules[*num_rules].pattern_size[rules[*num_rules].num_patterns] < MAX_PATTERN_LENGTH);
                rules[*num_rules].pattern[rules[*num_rules].num_patterns][rules[*num_rules].pattern_size[rules[*num_rules].num_patterns]++] = atoi(token);
            }
            token = strtok(NULL, " ");
        }
        rules[*num_rules].num_patterns += 1;

        *num_rules += 1;
    }

    // get the messages
    *num_messages = -1;
    while(fscanf(fp, "%s", messages[++*num_messages]) != EOF)
    {
        assert(*num_messages != MAX_MSG);
    }

    fclose(fp);
}

void build_rule_id_lookup(int num_rules, Rule_t rules[MAX_RULES], int rule_id_lookup[MAX_RULES])
{
    for(int i = 0; i < MAX_RULES; ++i)
    {
        rule_id_lookup[i] = -1;
    }

    for(int i = 0; i < num_rules; ++i)
    {
        rule_id_lookup[rules[i].id] = i;
    }
}

int msg_match(char *message, int msg_idx, int rule_index, Rule_t rules[MAX_RULES], int rule_id_lookup[MAX_RULES])
{
    // length check
    if(msg_idx >= strlen(message))
    {
        return 0;
    }

    // base case
    if(rules[rule_index].type == 1)
    {
        return (message[msg_idx] == rules[rule_index].c);
    }

    for(int i = 0; i < rules[rule_index].num_patterns; ++i)
    {
        int offset = 0;
        for(int j = 0; offset != -1 && j < rules[rule_index].pattern_size[i]; ++j)
        {
            int match = msg_match(message, msg_idx + offset, rule_id_lookup[rules[rule_index].pattern[i][j]], rules, rule_id_lookup);
            if(match != 0)
            {
                offset += match;
            }
            else
            {
                offset = -1;
            }
        }
        if(offset != -1)
        {
            return offset;
        }
    }
    return 0;
}

int part1(char *input_filename)
{
    int num_rules;
    Rule_t rules[MAX_RULES];
    int rule_id_lookup[MAX_RULES];
    int num_messages;
    char messages[MAX_MSG][MAX_STR_LEN];
    memset(messages, 0, MAX_MSG * MAX_STR_LEN * sizeof(char));

    get_input(input_filename, &num_rules, rules, &num_messages, messages);
    build_rule_id_lookup(num_rules, rules, rule_id_lookup);

    int count = 0;

    for(int i = 0; i < num_messages; ++i)
    {
        count += (msg_match(messages[i], 0, rule_id_lookup[0], rules, rule_id_lookup) == strlen(messages[i]));
    }

    return count;
}

// returns 1 if matched successfully, 0 otherwise
int msg_match2(char *message,
               int rule_list[MAX_STR_LEN], int rule_list_len,
               Rule_t rules[MAX_RULES], int rule_id_lookup[MAX_RULES])
{

    for(int i = 0; i < rule_list_len; ++i)
    {
        Rule_t rule = rules[rule_id_lookup[rule_list[i]]];
        if(rule.type == 1)
        {
            if(message[i] != rule.c)
            {
                return 0;
            }
        }
        else
        {
            // try expanding the rule into one of its constituent patterns
            // at a time, and continue the search
            for(int j = 0; j < rule.num_patterns; ++j)
            {
                if(rule_list_len + rule.pattern_size[j] - 1 <= strlen(message))
                {
                    assert(rule_list_len + rule.pattern_size[j] - 1 <= MAX_STR_LEN);
                    int rule_list_new[MAX_STR_LEN] = {0};
                    for(int k = 0; k < i; ++k)
                    {
                        rule_list_new[k] = rule_list[k];
                    }
                    for(int k = 0; k < rule.pattern_size[j]; ++k)
                    {
                        rule_list_new[i + k] = rule.pattern[j][k];
                    }
                    for(int k = i + rule.pattern_size[j]; k < MAX_STR_LEN; ++k)
                    {
                        rule_list_new[k] = rule_list[k - rule.pattern_size[j] + 1];
                    }
                    if(msg_match2(message, rule_list_new, rule_list_len + rule.pattern_size[j] - 1, rules, rule_id_lookup))
                    {
                        return 1;
                    }
                }
            }
            return 0;
        }
    }

    if(rule_list_len != strlen(message))
    {
        return 0;
    }

    // every character is matched correctly, no more, no less
    return 1;
}

int part2(char *input_filename)
{
    int num_rules;
    Rule_t rules[MAX_RULES];
    int rule_id_lookup[MAX_RULES];
    int num_messages;
    char messages[MAX_MSG][MAX_STR_LEN];
    memset(messages, 0, MAX_MSG * MAX_STR_LEN * sizeof(char));

    get_input(input_filename, &num_rules, rules, &num_messages, messages);
    build_rule_id_lookup(num_rules, rules, rule_id_lookup);

    assert(rule_id_lookup[8] != -1);
    rules[rule_id_lookup[8]].num_patterns = 2;
    rules[rule_id_lookup[8]].pattern_size[1] = 2;
    rules[rule_id_lookup[8]].pattern[1][0] = 42;
    rules[rule_id_lookup[8]].pattern[1][1] = 8;

    assert(rule_id_lookup[11] != -1);
    rules[rule_id_lookup[11]].num_patterns = 2;
    rules[rule_id_lookup[11]].pattern_size[1] = 3;
    rules[rule_id_lookup[11]].pattern[1][0] = 42;
    rules[rule_id_lookup[11]].pattern[1][1] = 11;
    rules[rule_id_lookup[11]].pattern[1][2] = 31;

    int count = 0;
    int rule_list[MAX_STR_LEN] = {0};

    for(int i = 0; i < num_messages; ++i)
    {
        count += msg_match2(messages[i], rule_list, 1, rules, rule_id_lookup);
    }

    return count;
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
