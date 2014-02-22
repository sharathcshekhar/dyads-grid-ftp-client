#include <stdio.h>

int get_metadata(char *path);

int main()
{	
	int ret = 0;
	ret = get_metadata("/");
	if (ret != 0) {
		printf("Failed\n");
		return ret;
	}
	printf("Suceeded\n");
	return 0;
}
