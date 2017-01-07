#ifndef DSTRING_H
#define DSTRING_H

#include <stddef.h>
#include <stdint.h>

struct dstring_api {
	/* Constructor and destructor */
	char *(*new)(size_t capacity);
	void (*delete)(char *self);

	/* Public methods */
	size_t (*length)(const char *self);
	char *(*assign)(char *self, const char *other, size_t length);
	char *(*append)(char *self, const char *other, size_t length);

	/* Constants */
	const size_t max_length;
};

extern const struct dstring_api String;

#endif
