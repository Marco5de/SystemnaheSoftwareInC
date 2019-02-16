#ifndef TOWNDB_H
#define TOWNDB_H

#include <stdbool.h>
#include "town.h"

typedef struct TownDB TownDB;

TownDB* create_towndb();
bool add_town(TownDB* db, Town* town);
void find_towns(TownDB* db, stralloc* key);
Town* find_next(TownDB* db);

#endif
