#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main()
{
	int fd[2];
	if (pipe(fd) != 0) {
		return 0;
	}
	int pid = fork();

	if (pid == 0) {
		/* child */
		printf("In child\n");
		dup2(fd[READ_END], 0);
		close(fd[WRITE_END]);
		close(fd[READ_END]);
		//char buf[10];
		//int ret = read(fd[READ_END], buf, 6);
		//printf("In child Read %d bytes, %s\n", ret, buf);
		execl("./child", "child", NULL);
	}

	else {
		int status;
		close(fd[READ_END]);
		char buf[10] = "hello";
		int ret	= write(fd[WRITE_END], "4\n", 2);
		printf("wrote %d bytes: %s\n", ret, buf);
		waitpid(pid, &status, 0);
	}
	return 0;
}
