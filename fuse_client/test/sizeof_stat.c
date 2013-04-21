#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
	struct stat fstat;
	printf("size of struct stat = %lu\n", sizeof(struct stat));
	printf("size of stat.st_dev = %lu\n", sizeof(fstat.st_dev));
	printf("size of stat.st_ino = %lu\n", sizeof(fstat.st_ino));
	printf("size of stat.st_mode = %lu\n", sizeof(fstat.st_mode));
	printf("size of stat.st_nlink = %lu\n", sizeof(fstat.st_nlink));
	printf("size of stat.st_uid = %lu\n", sizeof(fstat.st_uid));
	printf("size of stat.st_gid = %lu\n", sizeof(fstat.st_gid));
	printf("size of stat.st_rdev = %lu\n", sizeof(fstat.st_rdev));
	printf("size of stat.st_size = %lu\n", sizeof(fstat.st_size));
	printf("size of stat.st_blksize = %lu\n", sizeof(fstat.st_blksize));
	printf("size of stat.st_blocks = %lu\n", sizeof(fstat.st_blocks));
	printf("size of stat.st_atime = %lu\n", sizeof(fstat.st_atime));
	printf("size of stat.st_mtime = %lu\n",  sizeof(fstat.st_mtime));
	printf("size of stat.st_ctime = %lu\n", sizeof(fstat.st_ctime));
	return 0;
}
