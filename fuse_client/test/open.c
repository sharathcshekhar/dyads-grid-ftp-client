#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int fd = open("/tmp/fuse/hello", O_RDONLY);
	printf("Opened with file desc %d\n", fd);
	return 0;
}
