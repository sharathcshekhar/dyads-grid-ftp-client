#include <stdio.h>

char** dy_metadata_readdir(char *path);

int main()
{
	char **files = dy_metadata_readdir("/");
	return 0;
}
