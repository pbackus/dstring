#ifndef DSTRING_H
#define DSTRING_H

#include <stddef.h>
#include <stdint.h>

/* Constructor and destructor */
char *dstr_new(size_t capacity);
void dstr_delete(char *self);

/* Public methods */
size_t dstr_length(const char *self);
void dstr_assign(char **selfp, const char *other, size_t length);
void dstr_append(char **selfp, const char *other, size_t length);

/* Constants */
const size_t dstr_max_length;

#endif
