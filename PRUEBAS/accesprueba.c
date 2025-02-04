#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
	access("noexistente", F_OK);
	//perror("");
	printf("\n%d\n", errno);
	return 0;

}