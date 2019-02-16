#include <stdlib.h>
#include <stralloc.h>
#include "towndb.h"

#define BUCKET_SIZE (16384)

typedef size_t HashVal;

typedef struct Entry {
   stralloc keyword;
   Town* town;
   struct Entry* next;
} Entry;

struct TownDB {
   Entry* bucket[BUCKET_SIZE];
   /* iterating with find_towns & find_next: */
   Entry* it_entry;
   stralloc* it_key;
};

/* compute the hash value for a name */
static HashVal get_hashval(stralloc* name) {
   HashVal hashval = 0;
   for (size_t i = 0; i < name->len; ++i) {
      hashval = (hashval << 3) ^ name->s[i] ^ (hashval >> 28);
   }
   return hashval;
}

TownDB* create_towndb() {
   TownDB* db = calloc(1, sizeof(TownDB));
   return db;
}

static bool add_town_with_key(TownDB* db, Town* town, stralloc* key) {
   Entry* p = calloc(1, sizeof(Entry));
   if (!p) return false;
   stralloc_copy(&p->keyword, key);
   p->town = town;
   size_t index = get_hashval(key) % BUCKET_SIZE;
   p->next = db->bucket[index];
   db->bucket[index] = p;
   return true;
}

bool add_town(TownDB* db, Town* town) {
   bool ok = add_town_with_key(db, town, &town->name);
   if (!ok) return false;
   stralloc alias = {0};
   for (size_t i = 0; i < town->name.len; ++i) {
      if (town->name.s[i] == ' ') {
	 if (stralloc_copyb(&alias, town->name.s, i)) {
	    ok = add_town_with_key(db, town, &alias);
	    if (!ok) break;
	 }
      }
   }
   stralloc_free(&alias);
   return true;
}

void find_towns(TownDB* db, stralloc* key) {
   size_t index = get_hashval(key) % BUCKET_SIZE;
   db->it_entry = db->bucket[index];
   db->it_key = key;
}

Town* find_next(TownDB* db) {
   while (db->it_entry) {
      Entry* entry = db->it_entry;
      db->it_entry = db->it_entry->next;
      if (stralloc_diff(db->it_key, &entry->keyword) == 0) {
	 return entry->town;
      }
   }
   return 0;
}
