t_tree parseline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducciodo
int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, y finalmente liberar el nodo inicial recibido

execline

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
		execline(parseline(line));
		free(line);
	}
}