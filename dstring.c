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

/* new: allocate a new, empty string
 *
 * Public method
 */
static char *
new(size_t capacity)
{
	capacity = (capacity > MIN_CAPACITY) ? capacity : MIN_CAPACITY;
	struct string_header *p = malloc(sizeof *p + capacity + 1);

	if (!p) return NULL;

	p->capacity = capacity;
	p->length = 0;
	p->data[0] = '\0';

	return p->data;
}

/* delete: free all memory associated with a string
 *
 * Public method
 */
static void
delete(char *self)
{
	struct string_header *p = (struct string_header *) self - 1;
	free(p);
}

/* length: return the length of a string
 *
 * Public method
 */
static size_t
length(const char *self)
{
	struct string_header *p = (struct string_header *) self - 1;
	return p->length;
}

/* append: concatenate a string with another string or char[]
 *
 * If the resulting string would cause the allocation size to overflow
 * SIZE_MAX, it is silently truncated. The user is expected to check this
 * condition using String.length and String.max_length if they wish to avoid
 * truncation.
 *
 * Public method
 */
static char *
append(char *self, const char *other, size_t length)
{
	struct string_header *p = (struct string_header *) self - 1;
	size_t new_length;

	if (p->length > String.max_length - length) {
		new_length = String.max_length;
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

/* assign: replace a string's contents with those of another string or char[]
 *
 * Public method
 */
static char *
assign(char *self, const char *other, size_t length)
{
	struct string_header *p = (struct string_header *) self - 1;

	p->length = 0;
	return append(self, other, length);
}

const struct dstring_api String = {
	.new = new,
	.delete = delete,
	.length = length,
	.assign = assign,
	.append = append,
	.max_length = (SIZE_MAX - sizeof (struct string_header) - 1)
};
