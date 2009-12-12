/*
    Rule system

    Rules are short programs that make stuff happen in the game.  
    A rule responds to certain events (e.g. player movement) and changes the
    state of the game by calling hooks directly in the game logic (e.g. change
    player position).
    Global rules can respond to all events while local rules are only triggered
    on player contact, when the player's position matches the rule's position.
    This is helpful for elements such as doors (transport player) or NPCs
    (start conversation).
*/

#ifndef __RULES_H
#define __RULES_H

#include "defines.h"

typedef int(int, void*) RULE_CALLBACK;

typedef struct {
    short active; // 0=not
    short global; // 0=not; global rules get all possible events
    short x, y; // position on the map
    RULE_CALLBACK callback;
    const char* description;
} Rule;

extern void rules_init(void); // Sets the initial save state - the rules activated - for each map
extern void rules_load(int map); // Loads the current "rules activated" state for the given map - change by calling rules_save
extern void rules_save(int map); // Saves the current "rules activated" state for the given map - restore by calling rules_load
extern void rules_fire(int what, void* param); // Triggers an event and executes matching rules

#endif

