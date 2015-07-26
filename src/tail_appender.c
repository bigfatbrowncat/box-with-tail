#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_LENGTH		4

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: %s <target_binary_name> <tail_data_file_name>\n", argv[0]);
		exit(1);
	}
	
	FILE* target = fopen(argv[1], "ab");
	FILE* datafile = fopen(argv[2], "rb");
	
	fseek(target, 0L, SEEK_END);
	size_t target_length = ftell(target);
	printf("Appending %s to %s\n", argv[2], argv[1]);
	printf("Base target binary size (without tail) is: %ld\n", target_length);
	
	// Appending data
	while (!feof(datafile)) {
		char buf;
		fread(&buf, 1, 1, datafile);
		if (!feof(datafile)) {
			fwrite(&buf, 1, 1, target);
		}
	}
	fclose(datafile);
	
	// Appending magic
	char magic[MAGIC_LENGTH];
	magic[0] = 'T';
	magic[1] = 'A';
	magic[2] = 'I';
	magic[3] = 'L';
	fwrite(magic, 1, MAGIC_LENGTH, target);

	// Appending the target base size
	fwrite(&target_length, sizeof(size_t), 1, target);
	
	fclose(target);
	printf("Appending done\n");
}