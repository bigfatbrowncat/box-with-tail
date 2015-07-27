#include <stdio.h>
#include <stdlib.h>

#include "tail.h"

#define TAIL_MAGIC_LENGTH		4

size_t tail_data_start, tail_data_length;
FILE* tail_self = NULL;

int tail_open(char* binary_name) {
	if (tail_self != NULL) {
		return TAIL_ALREADY_OPENED;
	}
	tail_self = fopen(binary_name, "rb");
	
	fseek(tail_self, -(int)sizeof(size_t) - TAIL_MAGIC_LENGTH, SEEK_END);
	size_t data_end = ftell(tail_self);
	
	// Checking magic
	char magic[TAIL_MAGIC_LENGTH];
	fread(&magic, 1, TAIL_MAGIC_LENGTH, tail_self);
	if (magic[0] != 'T' || magic[1] != 'A' || magic[2] != 'I' || magic[3] != 'L') {
		//
		return TAIL_NOT_FOUND;
	}
	
	// Reading the base size
	fread(&tail_data_start, sizeof(size_t), 1, tail_self);
	tail_data_length = data_end - tail_data_start;
	
	return TAIL_SUCCESS;

}

size_t tail_get_length() {
	return tail_data_length;
}

void tail_read(char* ptr) {
	// Going to the base-size offset
	fseek(tail_self, tail_data_start, SEEK_SET);
	fread(ptr, 1, tail_data_length, tail_self);
}

void tail_close() {
	fclose(tail_self);
}
