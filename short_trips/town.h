#ifndef TOWN_H
#define TOWN_H

#include <stdio.h>
#include "coord.h"

typedef struct Town {
   stralloc name;
   Coordinates coord;
} Town;

Town* read_town(FILE* fp);

#endif
