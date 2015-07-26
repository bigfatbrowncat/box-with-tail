#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_LENGTH		4

#define BUF_SIZE			8

int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: %s <target_binary_name> <tail_data_file_name>\n", argv[0]);
		exit(1);
	}
	
	FILE* target = fopen(argv[1], "ab");
	FILE* datafile = fopen(argv[2], "rb");
	
	fseek(target, 0L, SEEK_END);
	size_t target_length = ftell(target);
	printf("Appending %s to %s at 0x%X\n", argv[2], argv[1], target_length);
	
	// Appending data
	size_t size = 0;
	while (!feof(datafile)) {
		char buf[BUF_SIZE];
		size_t rs = fread(&buf, 1, BUF_SIZE, datafile);
		if (rs > 0) {
			fwrite(&buf, 1, rs, target);
			size += rs;
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
	printf("Appended %ld bytes\n", size);
}