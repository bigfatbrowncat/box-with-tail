#include <stdio.h>
#include <stdlib.h>

#include "tail.h"

#define TAIL_MAGIC_LENGTH		4

int tail_error;

struct _tail_t {
	FILE* file;
	size_t tail_data_start;
	size_t tail_data_length;
};

int tail_prepare_reading(tail_t tail) {
	fseek(tail->file, tail->tail_data_start - (int)sizeof(size_t) - TAIL_MAGIC_LENGTH, SEEK_SET);
	size_t data_end = ftell(tail->file);
	
	// Checking magic
	char magic[TAIL_MAGIC_LENGTH];
	fread(&magic, 1, TAIL_MAGIC_LENGTH, tail->file);
	if (magic[0] != 'T' || magic[1] != 'A' || magic[2] != 'I' || magic[3] != 'L') {
		return 0;
	}
	
	// Reading the base size
	size_t data_start;
	fread(&data_start, sizeof(size_t), 1, tail->file);
	size_t tail_data_length = data_end - data_start;
	
	tail_error = TAIL_SUCCESS;
	
	tail->tail_data_start = data_start;
	tail->tail_data_length = tail_data_length;
	return 1;
}

// Public functions

tail_t tail_open(char* binary_name) {
	FILE* file = fopen(binary_name, "rb");
	if (file == NULL) {
		tail_error = TAIL_FILE_NOT_FOUND;
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);

	tail_t res = (tail_t)malloc(sizeof(struct _tail_t));
	res->tail_data_start = ftell(file);
	res->file = file;
	if (tail_prepare_reading(res) == 0) {
		free(res);
		res = NULL;
	}
	return res;
}

int tail_get_error() {
	return tail_error;
}

size_t tail_get_length(tail_t tail) {
	return tail->tail_data_length;
}

int tail_read_previous(tail_t tail, char* ptr) {
	// Going to the base-size offset
	fseek(tail->file, tail->tail_data_start, SEEK_SET);
	fread(ptr, 1, tail->tail_data_length, tail->file);
	
	return tail_prepare_reading(tail);
}

void tail_close(tail_t tail) {
	fclose(tail->file);
	free(tail);
}
