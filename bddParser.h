struct line_reader {
	/* All members are private. */
	FILE	*f;
	char	*buf;
	size_t	 siz;
};

void lr_init(struct line_reader *lr, FILE *f);
char * next_line(struct line_reader *lr, size_t *len);
void lr_free(struct line_reader *lr);
