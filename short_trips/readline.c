/*
 * Read a string of arbitrary length from a
 * given file pointer. LF is accepted as terminator.
 * 1 is returned in case of success, 0 in case of errors.
 * afb 4/2003
 */

#include "readline.h"

bool readline(FILE* fp, stralloc* sa) {
   sa->len = 0;
   for(;;) {
      if (!stralloc_readyplus(sa, 1)) return false;
      if (fread(sa->s + sa->len, sizeof(char), 1, fp) <= 0) return false;
      if (sa->s[sa->len] == '\n') break;
      ++sa->len;
   }
   return true;
}
