default: all

all:fuse
	
fuse: file_system.c
	gcc file_system.c -D_FILE_OFFSET_BITS=64 -DFUSE_USE_VERSION=22 -lfuse -o bin/filesystem

test: test/write.c
	gcc test/write.c -o test/bin/test_write

clean:
	rm -rf bin/*
	rm -rf test/bin/*

gftp: globus_client.c
	gcc -c -I/usr/lib/globus/include/ globus_client.c -o bin/globus_client.o
