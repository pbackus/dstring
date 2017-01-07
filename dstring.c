#include "dstring.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MIN_CAPACITY (sizeof (size_t)) /* For alignment */

struct string_header {
	size_t capacity;
	size_t length;
	char data[];
};

const size_t dstr_max_length = (SIZE_MAX - sizeof (struct string_header) - 1);

/* dstr_new: allocate a new, empty string
 *
 * Public method
 */
char *
dstr_new(size_t capacity)
{
	capacity = (capacity > MIN_CAPACITY) ? capacity : MIN_CAPACITY;
	struct string_header *p = malloc(sizeof *p + capacity + 1);

	if (!p) return NULL;

	p->capacity = capacity;
	p->length = 0;
	p->data[0] = '\0';

	return p->data;
}

/* dstr_delete: free all memory associated with a string
 *
 * Public method
 */
void
dstr_delete(char *self)
{
	struct string_header *p = (struct string_header *) self - 1;
	free(p);
}

/* dstr_length: return the length of a string
 *
 * Public method
 */
size_t
dstr_length(const char *self)
{
	struct string_header *p = (struct string_header *) self - 1;
	return p->length;
}

/* dstr_append: concatenate a string with another string or char[]
 *
 * If the resulting string would cause the allocation size to overflow
 * SIZE_MAX, it is silently truncated. The user is expected to check this
 * condition using dstr_length and dstr_max_length if they wish to avoid
 * truncation.
 *
 * Public method
 */
char *
dstr_append(char *self, const char *other, size_t length)
{
	struct string_header *p = (struct string_header *) self - 1;
	size_t new_length;

	if (p->length > dstr_max_length - length) {
		new_length = dstr_max_length;
	} else {
		new_length = p->length + length;
	}

	if (new_length > p->capacity) {
		/* TODO: better resize strategy to reduce reallocs */
		p = realloc(p, sizeof *p + new_length + 1);
		if (!p) goto error;

		p->capacity = new_length;
	}

	memmove(&(p->data[p->length]), other, length);
	p->length = new_length;
	p->data[p->length] = '\0';

	return p->data;

error:
	/* Out of memory */
	/* FIXME: is there a better way to handle this? */
	abort();
}

/* dstr_assign: replace a string's contents with those of another string or char[]
 *
 * Public method
 */
char *
dstr_assign(char *self, const char *other, size_t length)
{
	struct string_header *p = (struct string_header *) self - 1;

	p->length = 0;
	return dstr_append(self, other, length);
}
