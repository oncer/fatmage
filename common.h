#ifndef __COMMON_H
#define __COMMON_H

#include <allegro.h>
#include "headers.h"
#include "defines.h"

struct {
    BITMAP *tiles, *mage;
} Resource;

extern volatile int frames;

extern BITMAP* buffer;
extern void swap( void );

extern void error( const char* str );

#endif
