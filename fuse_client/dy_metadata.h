#include <sys/stat.h>

#define METADATA_PORT 6464
#define METADATA_IPADDR "127.0.0.1"
#define MAX_METADATA 512

typedef enum {
	DY_GET_ATTR = 1,
	DY_READ_DIR,
} dy_meta_msg_type_e;

typedef struct {
	uint8_t msg_type;
	uint32_t path_len;
	char *path;
} dy_meta_request_packet_t; 

char** dy_metadata_readdir(char *path);
int dy_meta_getattr(char *path, struct stat **fstat);
