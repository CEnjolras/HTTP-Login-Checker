#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bddParser.h"

/*
 * Initializes a line reader _lr_ for the stream _f_.
 */
void lr_init(struct line_reader *lr, FILE *f){
	lr->f = f;
	lr->buf = NULL;
	lr->siz = 0;
}
 
/*
 * Reads the next line. If successful, returns a pointer to the line,
 * and sets *len to the number of characters, at least 1. The result is
 * _not_ a C string; it has no terminating '\0'. The returned pointer
 * remains valid until the next call to next_line() or lr_free() with
 * the same _lr_.
 *
 * next_line() returns NULL at end of file, or if there is an error (on
 * the stream, or with memory allocation).
 */
char * next_line(struct line_reader *lr, size_t *len){
	size_t newsiz;
	int c;
	char *newbuf;
 
	*len = 0;			/* Start with empty line. */
	for (;;) {
		c = fgetc(lr->f);	/* Read next character. */
		if (ferror(lr->f))
			return NULL;
 
		if (c == EOF) {
			/*
			 * End of file is also end of last line,
		`	 * unless this last line would be empty.
			 */
			if (*len == 0)
				return NULL;
			else
				return lr->buf;
		} else {
			/* Append c to the buffer. */
			if (*len == lr->siz) {
				/* Need a bigger buffer! */
				newsiz = lr->siz + 4096;
				newbuf = realloc(lr->buf, newsiz);
				if (newbuf == NULL)
					return NULL;
				lr->buf = newbuf;
				lr->siz = newsiz;
			}
			lr->buf[(*len)++] = c;
 
			/* '\n' is end of line. */
			if (c == '\n')
				return lr->buf;
		}
	}
}
 
/*
 * Frees internal memory used by _lr_.
 */
void lr_free(struct line_reader *lr){
	free(lr->buf);
	lr->buf = NULL;
	lr->siz = 0;
}
