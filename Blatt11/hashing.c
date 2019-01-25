#include "hashing.h"
#include <stralloc.h>

#define HASHSTART 5381

unsigned int compute_hash(stralloc *s) {
   int hashval = HASHSTART;
   for (unsigned int index = 0; index < s->len; ++index) {
	  char ch = s->s[index];
	  hashval += hashval << 5;
	  hashval ^= ch;
   }
   return hashval;
}

