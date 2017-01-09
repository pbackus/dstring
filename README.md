dstring
=======

A simple dynamic string library for C.

- Strings know how long they are.
- Reallocation is handled automatically.
- Uses `char *` and NUL-termination for easy interop with libc.

Example
-------

Read an entire file into memory:

	#define array_size(a) (sizeof(a)/sizeof(a[0]))

	char *
	fslurp(FILE *input)
	{
	    char *result = dstr_new(0);
	    char buf[80];
	
	    while (fgets(buf, array_size(buf), input) != NULL) {
	
	        /* Check for overflow */
	        if (dstr_length(result) > dstr_max_length - strlen(buf)) {
	            fprintf(stderr, "Error: input exceeds maximum allowed size; "
	                    "truncating to first %zu bytes.", dstr_max_length);
	        }
	
	        dstr_append(&result, buf, strlen(buf));
	    }
	
	    if (ferror(input)) {
	        dstr_delete(result);
	        return NULL;
	    }
	
	    return result;
	}
