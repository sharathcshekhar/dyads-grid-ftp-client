#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "dy_metadata.h"

#define NTOHL_64(x) 	do {									\
							uint32_t lo = (uint32_t) (x >> 32); \
							uint32_t hi = (uint32_t) x;			\
							x = ntohl(hi);						\
							x <<=32;							\
							x |= ntohl(lo);						\
						} while(0)

static int send_packet(dy_meta_request_packet_t packet);
void print_stat(struct stat *fstat);
void print_stat(struct stat *fstat)
{
	FILE *fd = fopen("/tmp/fuse-logs", "a+");
	fprintf(fd, "stat.st_dev = %lu\n", fstat->st_dev);
	fprintf(fd, "stat.st_ino = %lu\n", fstat->st_ino);
	fprintf(fd, "stat.st_mode = %u\n", fstat->st_mode);
	fprintf(fd, "stat.st_nlink = %lu\n", fstat->st_nlink);
	fprintf(fd, "stat.st_uid = %u\n", fstat->st_uid);
	fprintf(fd, "stat.st_gid = %u\n", fstat->st_gid);
	fprintf(fd, "stat.st_rdev = %lu\n", fstat->st_rdev);
	fprintf(fd, "stat.st_size = %lu\n", fstat->st_size);
	fprintf(fd, "stat.st_blksize = %lu\n", fstat->st_blksize);
	fprintf(fd, "stat.st_blocks = %lu\n", fstat->st_blocks);
	fprintf(fd, "stat.st_atim.tv_sec = %lu\n", fstat->st_atim.tv_sec);
	fprintf(fd, "stat.st_atim.tv_nsec = %lu\n", fstat->st_atim.tv_nsec);
	fprintf(fd, "stat.st_mtim.tv_sec = %lu\n", fstat->st_mtim.tv_sec);
	fprintf(fd, "stat.st_mtim.tv_nsec = %lu\n", fstat->st_mtim.tv_nsec);
	fprintf(fd, "stat.st_ctim.tv_sec = %lu\n", fstat->st_ctim.tv_sec);
	fprintf(fd, "stat.st_ctim.tv_nsec = %lu\n", fstat->st_ctim.tv_nsec);
	fclose(fd);
	return;
}
int dy_meta_getattr(char *path, struct stat **fstat)
{
	dy_meta_request_packet_t req_pckt;
	uint8_t msg_type;
	uint32_t msg_len;
	uint32_t file_count;
	int sockfd;
	int ret = 0;
	char *recv_buf;
	
	req_pckt.msg_type = DY_GET_ATTR;
	req_pckt.path = path;
	req_pckt.path_len = htonl((int)strlen(path) + 1);
   	
	sockfd = send_packet(req_pckt);
	if (sockfd < 0) {
		printf("Failed to write packed\n");
		return -1;
	}
	ret = read(sockfd, &msg_type, 1);
	if (msg_type != DY_GET_ATTR) {
		printf("Error reading, ret = %d, msg_type = %d\n", ret, msg_type);
		return -1;
	}	
	
	ret = read(sockfd, &msg_len, 4);
	if (ret != 4) {
		printf("Error reading msg_len, ret = %d\n", ret);
		return -1;
	}
	msg_len = ntohl(msg_len); 
	printf("Msg len = %d\n", msg_len);
	recv_buf = calloc(512, 1);	

	ret = read(sockfd, recv_buf, 512);
	if (ret < 0) {
		printf("Read error in stat, ret = %d\n", ret);
		return -1;
	}
	
	(*fstat)->st_dev = 0;
	(*fstat)->st_ino = 0;
	(*fstat)->st_mode = *((uint32_t *)(recv_buf + 24));
	(*fstat)->st_nlink = 0; 
	(*fstat)->st_uid = 0;
	(*fstat)->st_gid = 0;
	(*fstat)->st_rdev = 0;
	(*fstat)->st_size = *((uint64_t*)(recv_buf + 48));
	(*fstat)->st_blksize = 0; 
	(*fstat)->st_blocks = 0;
	(*fstat)->st_atim.tv_sec = 0;
	(*fstat)->st_atim.tv_nsec = 0;
	(*fstat)->st_mtim.tv_sec = 0;
	(*fstat)->st_mtim.tv_nsec = 0;
	(*fstat)->st_ctim.tv_sec = 0;
	(*fstat)->st_ctim.tv_nsec = 0;
	
	NTOHL_64((*fstat)->st_dev);
	NTOHL_64((*fstat)->st_ino);
	(*fstat)->st_mode = ntohl((*fstat)->st_mode);
	NTOHL_64((*fstat)->st_nlink);
	(*fstat)->st_uid = ntohl((*fstat)->st_uid);
	(*fstat)->st_gid = ntohl((*fstat)->st_gid);
	NTOHL_64((*fstat)->st_rdev);
	NTOHL_64((*fstat)->st_size);
	NTOHL_64((*fstat)->st_blksize);
	NTOHL_64((*fstat)->st_blocks);
	NTOHL_64((*fstat)->st_atim.tv_sec);
	NTOHL_64((*fstat)->st_atim.tv_nsec);
	NTOHL_64((*fstat)->st_mtim.tv_sec);
	NTOHL_64((*fstat)->st_mtim.tv_nsec);
	NTOHL_64((*fstat)->st_ctim.tv_sec);
	NTOHL_64((*fstat)->st_ctim.tv_nsec);
	printf("in  get attr\n");
	//(*fstat)->st_mode |= S_IFDIR;
	print_stat(*fstat);
	return 0;
}

char **dy_metadata_readdir(char *path)
{
	dy_meta_request_packet_t req_pckt;
	uint8_t msg_type;
	uint32_t msg_len;
	uint32_t file_count;
	int sockfd;
	int ret;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	req_pckt.msg_type = DY_READ_DIR; 
	req_pckt.path = path;
	req_pckt.path_len = htonl((int)strlen(path) + 1);
   	
	sockfd = send_packet(req_pckt);
	ret = read(sockfd, &msg_type, 1);
	if (msg_type != DY_READ_DIR) {
		fprintf(fp,"Error reading, ret = %d, msg_type = %d\n", ret, msg_type);
		fflush(fp);
		return NULL;
	}	
	
	ret = read(sockfd, &file_count, 4);
	if (ret != 4) {
		fprintf(fp, "Error reading\n");
		fflush(fp);
		return NULL;
	}
	file_count = ntohl(file_count); 
	
	ret = read(sockfd, &msg_len, 4);
	if (ret != 4) {
		fprintf(fp, "Error reading\n");
		fflush(fp);
		return NULL;
	}
	msg_len = ntohl(msg_len); 
	
	fprintf(fp, "msg_type = %d, msg_len = %d, no of files = %d\n", msg_type, 
		   msg_len, file_count);	
	fflush(fp);
	
	char *recv_buf = malloc(msg_len);
	char **files = malloc(sizeof(char *) * (file_count + 1));
	int i = 0;
	int offset = 0;
	
	files[0] = recv_buf;
	while ((i < file_count) && (offset < msg_len)) {
		char msg;
		read(sockfd, &msg, 1);
		recv_buf[offset] = msg;
		offset++;
		if (msg == '\0') {
			fprintf(fp, "FILE READ: %s\n", files[i]);
			i++;
			files[i] = &recv_buf[offset];
		}
	}
	fprintf(fp, "Done with reads, file_count = %d\n", file_count);
	files[file_count] = NULL;
	fprintf(fp, "returning..\n");
	fflush(fp);
	fclose(fp);
	return files;
}

static int send_packet(dy_meta_request_packet_t packet)
{
	int sockfd = 0, ret = 0;
	struct sockaddr_in serv_addr; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
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

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Error : Connect Failed \n");
		return -1;
	}
	
	int len = 1;
	ret = write(sockfd, &packet.msg_type, len);
	if (ret != len) {
		printf("Error writing\n");
		return -1;
	}
	
	len = 4;
	printf("Writing path length %d %d %d %d\n", ((char*)&packet.path_len)[0],
				((char*)&packet.path_len)[1], ((char*)&packet.path_len)[2], ((char*)(&packet.path_len))[3]);
	ret = write(sockfd, &packet.path_len, len);
	if (ret != len) {
		printf("Error writing\n");
		return -1;
	}
	
	len = strlen(packet.path) + 1;	
	ret = write(sockfd, packet.path, len);
	if (ret != len) {
		printf("Error writing\n");
		return -1;
	}

	return sockfd;
}
