/* Map file constants */
#ifndef __DEFINES_H
#define __DEFINES_H

#define FMMF_VERSION 1
#define FMMF_OBJECTS 100

/* Tile constants */
#define TILE 16
#define T_EMPTY 0

/* Object constants */
#define O_NONE    0
#define O_TRIGGER 1

/* Object flags */
#define O_FLAG_SOLID 1

/* Screen constants */
#define WIDTH  400
#define HEIGHT 240
#define FRAMES_PER_SECOND 60

/* Player constants */
#define D_UP    1 // Walking direction
#define D_DOWN  0
#define D_RIGHT 2
#define D_LEFT  3
#define MAGE_ANIMLEN 6

/* Game constants */
#define G_STATE_NORMAL    0
#define G_STATE_TALKING   1
#define G_STATE_INVENTORY 2

/* Event constants */
#define E_POSITION_CHANGE 0

#endif

