#ifndef __MAGE_H
#define __MAGE_H

struct {
	int x, y;
	unsigned short walkc; // walk delay counter
	unsigned char dir; // direction
	unsigned char anim; // animation
	unsigned short animc; // animation delay counter
} mage;

#endif

