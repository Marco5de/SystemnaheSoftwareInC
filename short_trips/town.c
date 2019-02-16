#include <stdlib.h>
#include "readline.h"
#include "town.h"

bool readline(FILE* fp, stralloc* sa);

Town* read_town(FILE* fp) {
   stralloc line = {0};
   if (!readline(fp, &line)) {
      stralloc_free(&line);
      return 0;
   }
   char* field[3];
   field[0] = line.s; int fields = 1;
   for (size_t i = 0; i < line.len; ++i) {
      char* cp = line.s + i;
      if (*cp == ':') {
	 *cp = 0;
	 if (fields == 3) break;
	 field[fields++] = cp + 1;
      }
   }
   Coordinates coord;
   if (sscanf(field[1], "%lg", &coord.longitude) != 1 ||
	 sscanf(field[2], "%lg", &coord.latitude) != 1) {
      stralloc_free(&line);
      return 0;
   }
   Town* town = calloc(1, sizeof(Town));
   if (!town || !stralloc_copys(&town->name, field[0])) {
      if (town) free(town);
      stralloc_free(&line);
      return 0;
   }
   town->coord = coord;
   stralloc_free(&line);
   return town;
}
