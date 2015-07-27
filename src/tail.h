#define TAIL_SUCCESS			0
#define TAIL_NOT_FOUND			1
#define TAIL_ALREADY_OPENED		2

int		tail_open(char* binary_name);
size_t	tail_get_length();
void 	tail_read(char* ptr);
void 	tail_close();
