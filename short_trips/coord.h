#ifndef COORD_H
#define COORD_H

typedef struct {
   double latitude;
   double longitude;
} Coordinates;

double compute_distance(Coordinates a, Coordinates b);

#endif
