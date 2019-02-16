#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stralloc.h>
#include "coord.h"
#include "readline.h"
#include "town.h"
#include "towndb.h"

TownDB* load_towns(char* filename) {
   FILE* fp = fopen(filename, "r");
   if (!fp) return 0;
   TownDB* db = create_towndb();
   if (!db) {
      fclose(fp); return 0;
   }
   Town* town;
   while ((town = read_town(fp))) {
      add_town(db, town);
   }
   return db;
}

Town* find_town(TownDB* db, char* key) {
   stralloc s = {0}; stralloc_copys(&s, key);
   find_towns(db, &s);
   Town* first = find_next(db);
   if (!first) {
      fprintf(stderr, "No such town: %s\n", key);
      stralloc_free(&s);
      return 0;
   }
   Town* next = find_next(db);
   if (!next) {
      stralloc_free(&s);
      return first;
   }
   fprintf(stderr, "Name %s is ambigious:\n", key);
   fprintf(stderr, "%.*s\n", (int) first->name.len, first->name.s);
   while (next) {
      fprintf(stderr, "%.*s\n", (int) next->name.len, next->name.s);
      next = find_next(db);
   }
   stralloc_free(&s);
   return 0;
}

Town* select_town(TownDB* db, stralloc* key) {
   find_towns(db, key);
   Town* first = find_next(db);
   if (!first) {
      printf("No such town: %.*s\n", (int) key->len, key->s);
      return 0;
   }
   Town* next = find_next(db);
   if (!next) {
      return first;
   }
   printf("Name %.*s is ambigious:\n", (int) key->len, key->s);
   printf("%.*s\n", (int) first->name.len, first->name.s);
   while (next) {
      printf("%.*s\n", (int) next->name.len, next->name.s);
      next = find_next(db);
   }
   return 0;
}

int main(int argc, char** argv) {
   char* cmdname = *argv++; --argc;
   if (argc != 2) {
      fprintf(stderr, "Usage: %s start destination\n", cmdname);
      exit(1);
   }
   TownDB* db = load_towns("gemeinden.txt");
   if (!db) {
      fprintf(stderr, "%s: could not load table of towns.\n", cmdname);
      exit(1);
   }
   Town* start = find_town(db, argv[0]);
   Town* destination = find_town(db, argv[1]);
   if (!start || !destination) {
      fprintf(stderr, "%s: start and/or destination are not well-defined\n",
	 cmdname);
      exit(1);
   }
   if (start == destination) {
      fprintf(stderr, "%s: The journey appears rather short!\n", cmdname);
      exit(1);
   }
   printf("*** Long chain of short trips ***\n");
   printf("Your objective is to travel from %.*s to %.*s\n",
      (int) start->name.len, start->name.s,
      (int) destination->name.len, destination->name.s);
   printf("through a long chain of intermediate towns where\n");
   printf("the maximal distance between two consecutive towns\n");
   printf("of your journey is to be minimized.\n");

   Town* current = start; double max_distance = 0;
   stralloc line = {0};
   while (current != destination) {
      printf("You are currently located in %.*s.\n",
	 (int) current->name.len, current->name.s);
      Town* next;
      do {
	 printf("Next town? ");
	 if (!readline(stdin, &line)) {
	    printf("Bye!\n"); exit(1);
	 }
	 next = select_town(db, &line);
      } while (!next);
      double distance = compute_distance(current->coord, next->coord);
      printf("Distance from %.*s to %.*s: %4.1f km\n",
	 (int) current->name.len, current->name.s,
	 (int) next->name.len, next->name.s, distance);
      if (distance > max_distance) {
	 max_distance = distance;
	 printf("This is the new maximal distance!\n");
      }
      current = next;
   }
   printf("Welcome to %.*s!\n", (int) destination->name.len,
      destination->name.s);
   printf("Your maximal intermediate distance was %4.1f km\n",
      max_distance);
}
