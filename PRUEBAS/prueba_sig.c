#include "../minishell.h"
#include "../executor.h"
#include <errno.h>
#include "../GNL/get_next_line.h"

int main(void)
{
	char *line;


	while(1)
	{
		line = readline(">");
		printf("%s\n", line);
		free(line);
	}
	return (0);
}