#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main()
{
	FILE *fp = fopen("/tmp/fuse/hello", "r");
	char buf[128];
	fgets(buf, 128, fp);
	printf("%s\n", buf);
	printf("successfully read, attempting to write\n");
	int fd;
	fd = open("/tmp/fuse/hello", O_WRONLY);
	if (fd < 0) {
		printf("something went wrong\n");
		return 0;
	}
	write(fd, "write trolled\n", 32);
	printf("done write\n");
	return 0;
}
