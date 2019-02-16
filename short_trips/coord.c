#include <math.h>
#include "coord.h"

double square(double x) {
   return x * x;
}

#define	PI 3.14159265358979323846

/* formula taken from https://de.wikipedia.org/wiki/Orthodrome */
double compute_distance(Coordinates a, Coordinates b) {
   static const double f = 1/298.257223563;
   static const double ar = 6378.137;
   double F = (a.latitude + b.latitude) / 2;
   double G = (a.latitude - b.latitude) / 2;
   double l = (a.longitude - b.longitude) / 2;

   /* convert F, G and l into radians */
   F = PI/180 * F; G = PI/180 * G; l = PI/180 * l;

   double S = square(sin(G)) * square(cos(l)) +
              square(cos(F)) * square(sin(l));
   double C = square(cos(G)) * square(cos(l)) +
              square(sin(F)) * square(sin(l));
   double w = atan(sqrt(S/C));
   double D = 2 * w * ar;
   double T = sqrt(S*C) / w;
   double H1 = (3*T - 1)/(2*C);
   double H2 = (3*T + 1)/(2*S);
   double s = D * (1 + f*H1*square(sin(F))*square(cos(G)) -
                       f*H2*square(cos(F))*square(sin(G)));
   return s;
}
