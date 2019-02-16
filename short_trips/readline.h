#ifndef READLINE_H
#define READLINE_H

#include <stralloc.h>
#include <stdio.h>
#include <stdbool.h>

bool readline(FILE* fp, stralloc* sa);

#endif
