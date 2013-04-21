#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static int dy_fuse_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in hello_getattr, path: %s\n", path);
	fclose(fp);
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}
	else if(strcmp(path, hello_path) == 0) {
		stbuf->st_mode = S_IFREG | 0666;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	}
	else if(strcmp(path, "/second-file") == 0) {
		stbuf->st_mode = S_IFREG | 0666;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen("fuse troll\n");
	} else
		res = -ENOENT;

	return res;
}

static int dy_fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in hello_readattr path: %s\n", path);
	fclose(fp);
	struct stat *stbuf = calloc(sizeof(struct stat), 1);
    memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);
	
	stbuf->st_mode = S_IFREG | 0666;
	stbuf->st_nlink = 1;
	stbuf->st_size = strlen("fuse troll\n");
    filler(buf, "second-file", NULL, 0);
    //filler(buf, "second-file", stbuf, 0);

    return 0;
}

static int dy_fuse_open(const char *path, struct fuse_file_info *fi)
{
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in hello_open\n");
	fclose(fp);
  //  if (strcmp(path, hello_path) != 0)
  //      return -ENOENT;

    if ((fi->flags & 3) != O_RDONLY) {
		fp = fopen("/tmp/fuse-logs", "a+");
		fprintf(fp, "in hello_open: opening for write mode\n");
		fclose(fp);
        //return -EACCES;
	}

    return 0;
}

static int dy_fuse_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in hello_read\n");
	fclose(fp);
    /*
	if(strcmp(path, hello_path) != 0)
        return -ENOENT;
	*/

	if(strcmp(path, hello_path) == 0) {
    	len = strlen(hello_str);
    	if (offset < len) {
    	    if (offset + size > len)
    	        size = len - offset;
    	    memcpy(buf, hello_str + offset, size);
    	} else
     	   size = 0;
	}
	if(strcmp(path, "/second-file") == 0) {
    	len = strlen("fuse troll\n");
    	if (offset < len) {
    	    if (offset + size > len)
    	        size = len - offset;
    	    memcpy(buf, "fuse troll\n" + offset, size);
    	} else
        	size = 0;
	}
    return size;
}

static int dy_fuse_write(const char *path, const char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) 
{
	FILE *fp = fopen("/tmp/fuse-logs", "a+");
	fprintf(fp, "in hello_write\n");
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
    return fuse_main(argc, argv, &dyads_oper);
}
