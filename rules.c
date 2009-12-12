#include "rules.h"
#include <string.h>
#include <stdio.h>

Rule rules[] = {
    { 1, 1, 0, 0, steplimit_rule, "Keeps track of the mage's step count and makes him slower as he gets exhausted." },
};

// Vectors of active rules (0 means not) for each map
const char* map_rules[] = {
    "0", // map 1
    "0", // map 2
    "0", // map 3
    "0", // map 4
    "0", // map 5
    "0", // map 6
    "0", // map 7
    "0", // map 8
    "0", // map 9
    "0", // map 0
}

void rules_init(void)
{
    map_rules[0] = "1"; // map 1
    map_rules[1] = "0"; // map 2
    map_rules[2] = "0"; // map 3
    map_rules[3] = "0"; // map 4
    map_rules[4] = "0"; // map 5
    map_rules[5] = "0"; // map 6
    map_rules[6] = "0"; // map 7
    map_rules[7] = "0"; // map 8
    map_rules[8] = "0"; // map 9
    map_rules[9] = "0"; // map 0
}

void rules_load(int map)
{
    assert(map >= 0 && map < 10);
    const char* vec = map_rules[map];
    for (int i=0; i<strlen(vec); ++i) {
        int state = vec[i] - '0';
        rules[i].active = state;
    }
}

void rules_save(int map)
{
    assert(map >= 0 && map < 10);
    const char* vec = map_rules[map];
    for (int i=0; i<strlen(vec); ++i) {
        int state = rules[i].active + '0';
        vec[i] = state;
    }
}

void rules_fire(int what, void* param)
{
    int rules_count = strlen(map_rules[0]); // A kinda whacky approach to determine the number of rules
    for (int i=0; i<rules_count; ++i)
    {
        if (rules[i].active && rules[i].global)
        {
            rules[i].callback(what, param);
        } 
        else if (rules[i].active && !roles[i].global 
            && event == E_POSITION_CHANGE)
        {
            //TODO: check with mage position
        }
    }
}

static int steplimit_rule(int what, void* param)
{
    switch (what)
    {
        case E_POSITION_CHANGE:
            fprintf(stderr, "Position change in steplimit_rule\n");
            break;
        default: assert(0);
    }
}

