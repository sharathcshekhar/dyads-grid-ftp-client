#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "dy_metadata.h"
#include "dy_gftp.h"
#include <libgen.h>

#define READ_END 0
#define WRITE_END 1
#define UBER_FTP "uberftp"

int fd[2];

#define u2d "/tmp/dyads-uber-fifo"
#define MAX_FILENAME 128

struct file_object {
	char name[MAX_FILENAME];
	int local_fd;
};

struct file_object file_table[10];

static int dy_fuse_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in dy_fuse_getattr, path: %s\n", path);
	memset(stbuf, 0, sizeof(struct stat));
	struct stat *fstat;
	
	res = dy_meta_getattr(path, &stbuf);
	fprintf(fp, "in dy_fuse_getattr, stbuf->st_mode: 0x%x, return value = %d\n", stbuf->st_mode, res);
	
	if(strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		stbuf->st_size = 0;
	}
	
	fclose(fp);
	return res;
}

static int dy_fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in dy_fuse_readdir path: %s\n", path);
	struct stat *stbuf = calloc(sizeof(struct stat), 1);
    memset(stbuf, 0, sizeof(struct stat));
	
	fprintf(fp, "in dy_fuse_readdir read dir lising:1\n");
	fflush(fp);
	char **files = dy_metadata_readdir(path); 
	fprintf(fp, "in dy_fuse_readdir read dir lising:2\n");
	fflush(fp);
	int i;
	for (i = 0; files[i] != NULL; i++) {
		fprintf(fp, "in dy_fuse_readdir file: %s\n", files[i]);
		fflush(fp);
		filler(buf, files[i], NULL, 0);
	}
	fclose(fp);
    return 0;
}

static int dy_fuse_open(const char *path, struct fuse_file_info *fi)
{
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "open()\n");
	fclose(fp);

    if ((fi->flags & 3) != O_RDONLY) {
		fp = fopen("/tmp/fuse-logs", "a+");
		fprintf(fp, "open: opening for write mode\n");
		fclose(fp);
	}

	fp = fopen("/tmp/fuse-logs", "a+");
	char buf[256];
	//path = basename(path);
	sprintf(buf, "%s %s %s\n", "get", (path + 1), "/tmp/dyads-cache/");
	int ret	= write(fd[WRITE_END], buf, strlen(buf));
	fprintf(fp, "open: Wrote %d bytes, %s\n", ret, buf);
	fflush(fp);
	char tmp_filename[MAX_FILENAME];
	path = basename(path);
	sprintf(tmp_filename, "%s%s", "/tmp/dyads-cache/", path);
	char status;
	fprintf(fp, "Blocking now.. \n");
	int block_fd = open(u2d, O_RDONLY);
	read(block_fd, &status, 1);
	fprintf(fp, "Unblocked.. %c\n", status);
	fi->fh = open(tmp_filename, fi->flags);
	fprintf(fp, "Allocated local fd %ld\n", fi->fh);
	fclose(fp);
	return 0;
}

static int dy_fuse_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in dy_fuse_read\n");
    
	fprintf(fp, "read called with fd %ld\n", fi->fh);
	lseek(fi->fh, offset, SEEK_SET);
	size = read(fi->fh, buf, size);
	fclose(fp);
    return size;
}

static int dy_fuse_write(const char *path, const char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) 
{
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in dy_fuse_write\n");
	fprintf(fp, "write called with fd %ld\n", fi->fh);
	lseek(fi->fh, offset, SEEK_SET);
	size = write(fi->fh, buf, size);
	char cmd[256];
	char *filename = basename(path);
	sprintf(cmd, "%s %s%s %s\n", "put", "/tmp/dyads-cache/", filename, path+1);
	int ret	= write(fd[WRITE_END], cmd, strlen(cmd));
	fprintf(fp, "wrote %d bytes by sending the command %s\n", ret, cmd);
	fflush(fp);
	fclose(fp);
	return 0;
}

static struct fuse_operations dyads_oper = {
	.getattr = dy_fuse_getattr,
	.readdir = dy_fuse_readdir,
	.open = dy_fuse_open,
	.read = dy_fuse_read,
	.write = dy_fuse_write,
};

int main(int argc, char *argv[])
{
	if (pipe(fd) != 0) {
		return 0;
	}
	mkfifo(u2d, 0666);
	int child_pid = fork();
	if (child_pid == 0) {
		printf("In child\n");
		dup2(fd[READ_END], 0);
		close(fd[WRITE_END]);
		close(fd[READ_END]);
		int ret = execl(UBER_FTP, UBER_FTP, NULL);
		printf("deadman walking, ret = %d\n", ret);
	} else {
		int pid = getpid();
		close(fd[READ_END]);
		printf("My pid = %d\n", pid);
		char cmd[100]; 
		sprintf(cmd, "%s %s\n","open", GRID_SERVER_URL);
		mkdir("/tmp/dyads-cache", 0755);
		write(fd[WRITE_END], cmd, strlen(cmd));	
		int ret = fuse_main(argc, argv, &dyads_oper);
		printf("fuse main returned the value %d\n", ret);
		return ret;
	}
}
