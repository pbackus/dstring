dstring
=======

A simple dynamic string library for C.

- Strings know how long they are.
- Reallocation is handled automatically.
- Uses `char *` and NUL-termination for easy interop with libc.

Example
-------

Read an entire file into memory:

	char *
	fslurp(FILE *input)
	{
	    char *result = String.new(0);
	    char linebuf[80]; /* Width of a standard terminal */
	
	    while (fgets(linebuf, array_size(linebuf), input) != NULL) {
	
	        /* Check for overflow */
	        if (String.length(result) > DSTRING_MAX_LEN - strlen(linebuf)) {
	            fprintf(stderr, "Error: input exceeds maximum allowed size; "
	                    "truncating to first %zu bytes.", DSTRING_MAX_LEN);
	        }
	
	        result = String.append(result, linebuf, strlen(linebuf));
	    }
	
	    if (ferror(input)) {
	        String.delete(result);
	        return NULL;
	    }
	
	    return result;
	}
