#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define METADATA_PORT 4444
#define METADATA_IPADDR "192.168.1.10"
#define MAX_METADATA 1024

int get_metadata(char *path)
{
	int sockfd = 0, n = 0;
	char recv_buf[MAX_METADATA];
	struct sockaddr_in serv_addr; 
	int ret = 0;

	memset(recv_buf, '0',sizeof(recv_buf));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error : Could not create socket \n");
		return -1;
	}
	
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(METADATA_PORT);
	if (inet_pton(AF_INET, METADATA_IPADDR, &serv_addr.sin_addr) <= 0) {
		printf("inet_pton error occured\n");
		return -1;
	}

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Error : Connect Failed \n");
		return -1;
	}
	ret = write(sockfd, path, strlen(path));
	if (ret != strlen(path)) {
		return -1;
	}
	while ((n = read(sockfd, recv_buf, sizeof(recv_buf) - 1)) > 0) {
		if (n < 0) {
			printf("Read error \n");
		}
		recv_buf[n] = '\0';
	}
	printf("Reply: %s\n", recv_buf);
//	struct stat fstat = (struct stat *)recv_buf;
//	print_stat(fstat);
	return 0;
}

#if 0
void print_stat(struct stat fstat)
{
dev_t     st_dev;     /* ID of device containing file */
ino_t     st_ino;     /* inode number */
mode_t    st_mode;    /* protection */
nlink_t   st_nlink;   /* number of hard links */
uid_t     st_uid;     /* user ID of owner */
gid_t     st_gid;     /* group ID of owner */
dev_t     st_rdev;    /* device ID (if special file) */
off_t     st_size;    /* total size, in bytes */
blksize_t st_blksize; /* blocksize for file system I/O */
blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
time_t    st_atime;   /* time of last access */
time_t    st_mtime;   /* time of last modification */
time_t    st_ctime;   /* time of last status change */
}
#endif
