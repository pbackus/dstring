#ifndef DSTRING_H
#define DSTRING_H

#include <stddef.h>
#include <stdint.h>

/* Constructor and destructor */
char *dstr_new(size_t capacity);
void dstr_delete(char *self);

/* Public methods */
size_t dstr_length(const char *self);
char *dstr_assign(char *self, const char *other, size_t length);
char *dstr_append(char *self, const char *other, size_t length);

/* Constants */
const size_t dstr_max_length;

#endif
