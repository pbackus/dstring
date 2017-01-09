#include "dstring.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct string_header {
	size_t capacity;
	size_t length;
	char data[];
};

#define MIN_CAPACITY (sizeof (size_t)) /* For alignment */

const size_t dstr_max_length = (SIZE_MAX - sizeof (struct string_header) - 1);

/* dstr_new: allocate a new, empty string
 *
 * Public method
 */
char *
dstr_new(size_t capacity)
{
	capacity = (capacity > MIN_CAPACITY) ? capacity : MIN_CAPACITY;
	struct string_header *h = malloc(sizeof *h + capacity + 1);

	if (!h) return NULL;

	h->capacity = capacity;
	h->length = 0;
	h->data[0] = '\0';

	return h->data;
}

/* dstr_delete: free all memory used by a string
 *
 * Public method
 */
void
dstr_delete(char *self)
{
	struct string_header *h = (struct string_header *) self - 1;
	free(h);
}

/* dstr_length: return the length of a string
 *
 * Public method
 */
size_t
dstr_length(const char *self)
{
	struct string_header *h = (struct string_header *) self - 1;
	return h->length;
}

/* dstr_append: append the contents of a char[] to a string
 *
 * If doing so would cause the allocation size to exceed SIZE_MAX, it is
 * silently truncated. The user is expected to check this condition using
 * dstr_length and dstr_max_length if they wish to avoid truncation.
 *
 * Public method
 */
void
dstr_append(char **selfp, const char *other, size_t length)
{
	struct string_header *h = (struct string_header *) *selfp - 1;
	size_t new_length;

	if (h->length > dstr_max_length - length) {
		new_length = dstr_max_length;
	} else {
		new_length = h->length + length;
	}

	if (new_length > h->capacity) {
		/* TODO: better resize strategy to reduce reallocs */
		h = realloc(h, sizeof *h + new_length + 1);
		if (!h) goto error;

		h->capacity = new_length;
	}

	memmove(&(h->data[h->length]), other, length);
	h->length = new_length;
	h->data[h->length] = '\0';

	*selfp = h->data;
	return;

error:
	/* Out of memory */
	/* FIXME: is there a better way to handle this? */
	abort();
}

/* dstr_assign: replace a string's contents with those of another char[]
 *
 * Public method
 */
void
dstr_assign(char **selfp, const char *other, size_t length)
{
	struct string_header *h = (struct string_header *) *selfp - 1;

	h->length = 0;
	dstr_append(selfp, other, length);
}
