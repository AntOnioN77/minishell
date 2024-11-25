int main(int argc, char **argv, char **envp)
{
	char *line;
	while(1)
	{
		line = readline("mini$hell>");
		if(!line)
		{
			perror("readline:");
			break;
		}
		execeline(parseline(line));
		free(line);
	}
}