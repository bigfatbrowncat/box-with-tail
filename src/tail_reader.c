#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tail.h"

int main(int argc, char** argv) {
	int res;
	printf("-- %s\n", argv[0]);
	tail_t tail_self = tail_open(argv[0]);
	if (tail_self != NULL) {
		int i = 0;
		int can_read_more = 1;
		while (can_read_more) {
			size_t data_length = tail_get_length(tail_self);
			char* data = (char*)malloc(data_length + 1);
			can_read_more = tail_read_previous(tail_self, data);

			i++;
			data[data_length] = 0;
			printf("[%d] Tail data: \"%s\" (%ld bytes)\n", i, data, data_length);
			free(data);
		}

		tail_close(tail_self);
	} else {
		printf("Fatal error: Eeyore can't find his tail. He's very sad and disappointed\n");
		res = 1;
	}
	printf("\n");
	return res;
}