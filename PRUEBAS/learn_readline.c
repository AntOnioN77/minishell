
#include "munit/munit.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(void)
{
	char *input;
	input = readline("Texto:");
	
	printf("%s", input);
}