#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_LENGTH		4

int main(int argc, char** argv) {
	FILE* self = fopen(argv[0], "rb");

	fseek(self, -(int)sizeof(size_t) - MAGIC_LENGTH, SEEK_END);
	size_t data_end = ftell(self);
	
	// Checking magic
	char magic[MAGIC_LENGTH];
	fread(&magic, 1, MAGIC_LENGTH, self);
	if (magic[0] != 'T' || magic[1] != 'A' || magic[2] != 'I' || magic[3] != 'L') {
		printf("Fatal error: No tail found. Add a tail to proceed.\n");
		exit(1);
	}

	// Reading the base size
	size_t data_start;
	fread(&data_start, sizeof(size_t), 1, self);
	printf("Tail data starts at: %d\n", data_start);
	
	// Going to the base-size offset
	fseek(self, data_start, SEEK_SET);
	size_t data_length = data_end - data_start;
	printf("Tail data length is: %d\n", data_length);
	
	char* data = (char*)malloc(data_length + 1);
	fread(data, 1, data_length, self);
	data[data_length] = 0;
	printf("Tail data: \"%s\"", data);
}