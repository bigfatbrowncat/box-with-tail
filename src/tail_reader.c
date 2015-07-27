#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tail.h"

int main(int argc, char** argv) {
	int res;
	int itres = tail_open(argv[0]);
	if (itres == TAIL_SUCCESS) {
		size_t data_length = tail_get_length();
		char* data = (char*)malloc(data_length + 1);
		tail_read(data);
		data[data_length] = 0;
		
		printf("Tail data: \"%s\" (%ld bytes)\n", data, data_length);
		
		free(data);
		res = 0;
	} else {
		printf("Fatal error: Eeyore can't find his tail. He's very sad and disappointed\n");
		res = 1;
	}
	tail_close();
}