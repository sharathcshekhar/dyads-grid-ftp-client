#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
	struct __attribute__((__packed__)) stat *fstat;
	struct __attribute__((__packed__)) stat f1stat;
	printf("size of struct stat = %lu\n", sizeof(f1stat));
	printf("size of stat.st_dev = %lu\n", sizeof(fstat->st_dev));
	printf("size of stat.st_ino = %lu\n", sizeof(fstat->st_ino));
	printf("size of stat.st_mode = %lu\n", sizeof(fstat->st_mode));
	printf("size of stat.st_nlink = %lu\n", sizeof(fstat->st_nlink));
	printf("size of stat.st_uid = %lu\n", sizeof(fstat->st_uid));
	printf("size of stat.st_gid = %lu\n", sizeof(fstat->st_gid));
	printf("size of stat.st_rdev = %lu\n", sizeof(fstat->st_rdev));
	printf("size of stat.st_size = %lu\n", sizeof(fstat->st_size));
	printf("size of stat.st_blksize = %lu\n", sizeof(fstat->st_blksize));
	printf("size of stat.st_blocks = %lu\n", sizeof(fstat->st_blocks));
	printf("size of stat.st_atim = %lu\n", sizeof(fstat->st_atim));
	printf("size of stat.st_mtim = %lu\n",  sizeof(fstat->st_mtim));
	printf("size of stat.st_ctim = %lu\n", sizeof(fstat->st_ctim));
	
	printf("Offset of stat.st_ctim = %ld\n", (char*)&(f1stat.st_ctim) - (char*)&f1stat);
	return 0;
}
