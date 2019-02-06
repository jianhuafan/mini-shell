/* Author: Robbert van Renesse 2018
 *
 * The interface is as follows:
 *	reader_t reader_create(int fd);
 *		Create a reader that reads characters from the given file descriptor.
 *
 *	char reader_next(reader_t reader):
 *		Return the next character or -1 upon EOF (or error...)
 *
 *	void reader_free(reader_t reader):
 *		Release any memory allocated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "shall.h"

#define BUFFER_LENGTH 512

struct reader {
	int fd;
	int idx;
	char buffer[BUFFER_LENGTH];
	int nbyte;
};

reader_t reader_create(int fd){
	reader_t reader = (reader_t) calloc(1, sizeof(*reader));
	reader->fd = fd;
	reader->idx = 0;
	reader->nbyte = 0;
	return reader;
}

char reader_next(reader_t reader){
	if (reader->idx < reader->nbyte - 1) {
		return reader->buffer[++reader->idx];
	} else {
		reader->nbyte = read(reader->fd, reader->buffer, BUFFER_LENGTH);
		reader->idx = 0;
		if (reader->nbyte <= 0) {
			return EOF;
		}
		return reader->buffer[0];
	}
}

void reader_free(reader_t reader){
	free(reader);
}
