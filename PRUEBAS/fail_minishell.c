//t_tree parseline(char *line)//debe retornar un arbol con un nodo para cada fraccion del comando introducciodo
//int execline(t_tree)//debe ejecutar y liberar cada nodo del arbol, y finalmente liberar el nodo inicial recibido

//execline

void command_flow(char **envp)
{
	char *line;
	t_tree parsline;

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

/*Este enfoque fracasa por que no podemos usar setenv, no tiene sentico
crear una copia de las variables de entorno mediante fork poara despues
volver a clonar envp para aplicar cambios*/
int main(int argc, char **argv, char **envp)
{
	int wstatus;

	if(fork()==0) // este fork sirve para evitar modificar las variables de entorno originales
	{
		upgrade_envp(envp);
		command_flow(envp);// contiene una copia (hijo) de envp
	}
	if(wait(&wstatus)==-1)
	printf("main/wait terminado por una señal, o no hay procesos hijos"); //Solo debugging
	return(wstatus);
}