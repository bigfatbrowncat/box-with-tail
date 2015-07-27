#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAIL_MAGIC_LENGTH		4

#define TAIL_SUCCESS			0
#define TAIL_NOT_FOUND			1

size_t tail_data_start, tail_data_length;
FILE* tail_self;

int init_tail(int argc, char** argv) {
	tail_self = fopen(argv[0], "rb");
	
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
	
	fclose(tail_self);
	return TAIL_SUCCESS;

}

size_t get_tail_length() {
	return tail_data_length;
}

void read_tail(char* ptr) {
	// Going to the base-size offset
	fseek(tail_self, tail_data_start, SEEK_SET);
	fread(ptr, 1, tail_data_length, tail_self);
}

void free_tail() {
	fclose(tail_self);
}

int main(int argc, char** argv) {
	int res;
	int itres = init_tail(argc, argv);
	if (itres == TAIL_SUCCESS) {
		size_t data_length = get_tail_length();
		char* data = (char*)malloc(data_length + 1);
		read_tail(data);
		data[data_length] = 0;
		
		printf("Tail data: \"%s\" (%ld bytes)\n", data, data_length);
		
		free(data);
		res = 0;
	} else {
		printf("Fatal error: Eeyore can't find his tail. He's very sad and disappointed\n");
		res = 1;
	}
	free_tail();
}