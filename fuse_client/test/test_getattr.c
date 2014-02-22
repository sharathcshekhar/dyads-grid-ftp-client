#include <stdio.h>
#include <sys/stat.h>
void print_stat(struct stat *fstat);
int dy_meta_getattr(char *path, struct stat **fstat);

int main()
{
	struct stat *fstat;
	int ret = dy_meta_getattr("/", &fstat);
	if (ret != 0) {
		printf("Failed\n");
		return -1;	
	}
	print_stat(fstat);
	return 0;
}

void print_stat(struct stat *fstat)
{
	printf("stat.st_dev = %lu\n", fstat->st_dev);
	printf("stat.st_ino = %lu\n", fstat->st_ino);
	printf("stat.st_mode = %u\n", fstat->st_mode);
	printf("stat.st_nlink = %lu\n", fstat->st_nlink);
	printf("stat.st_uid = %u\n", fstat->st_uid);
	printf("stat.st_gid = %u\n", fstat->st_gid);
	printf("stat.st_rdev = %lu\n", fstat->st_rdev);
	printf("stat.st_size = %lu\n", fstat->st_size);
	printf("stat.st_blksize = %lu\n", fstat->st_blksize);
	printf("stat.st_blocks = %lu\n", fstat->st_blocks);
	printf("stat.st_atim.tv_sec = %lu\n", fstat->st_atim.tv_sec);
	printf("stat.st_atim.tv_nsec = %lu\n", fstat->st_atim.tv_nsec);
	printf("stat.st_mtim.tv_sec = %lu\n", fstat->st_mtim.tv_sec);
	printf("stat.st_mtim.tv_nsec = %lu\n", fstat->st_mtim.tv_nsec);
	printf("stat.st_ctim.tv_sec = %lu\n", fstat->st_ctim.tv_sec);
	printf("stat.st_ctim.tv_nsec = %lu\n", fstat->st_ctim.tv_nsec);
	return;
}
