#include<stdio.h>
int main()
{
	int a;
	while(1){
		printf("Tryin to read\n");
		scanf("%d", &a);
		printf("read %d from stdin\n", a);
	}
	return 0;
}
