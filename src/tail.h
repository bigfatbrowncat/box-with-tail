#define TAIL_SUCCESS			0
#define TAIL_FILE_NOT_FOUND		1
#define TAIL_TAIL_NOT_FOUND		2

typedef struct _tail_t* tail_t;

tail_t		tail_open(char* binary_name);
size_t		tail_get_length(tail_t tail);
int			tail_get_error();
int 		tail_read_previous(tail_t tail, char* ptr);
void 		tail_close(tail_t tail);
