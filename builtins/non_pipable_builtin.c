//Hay buitins que en el contexto de minishell no tiene sentido que formen parte de un pipe
//Son: cd, export, unset y exit.
//la solucion mas directa es ejecutarlos solo cuando el arbol sea unicamente de un nodo

#include "minishell.h"

static int countargs(t_task *task)
{
	int i;

	if(!task || !task->argv)
		return (0);
	i = 0;
	while(task->argv[i])
		i++;
	return (i);
}

/*
static char *getnamevar()
{
	//probablemente ya hay una funcion que hace esto
}
*/

char *getvalue(char *var)
{	
	char *start;

	start = ft_strchr(var, '=');
	if (!start)
		return(NULL);
	start++;
	return(ft_strdup(start));
	
}

void export_error(char* identifier, t_environ *environ)
{
	char *atomic;
	char *aux;

	aux = ft_strjoin("mini$hell: export: `", identifier);
	atomic = ft_strjoin(aux, "': not a valid identifier\n");
	free(aux);
	ft_putstr_fd(atomic, 2);
	free(atomic);
	change_var("$", "1", environ);
}

//retorna 1 si key es valido (el primer caracter es alpafabetico, contiene solo caracteres alphanumericos y no es una cadena vacía)
int validate_key(char *key)
{
	int i;
	if(!ft_strcmp(key, ""))
		return(0);
	if(!ft_isalpha(key[0]))
			return(0);
	i =1;
	while (key[i])
	{
		if(!ft_isalnum(key[i]))
			return(0);
		i++;
	}
	return (1);
}

//export no debe cambiar variables como $? $PWD o $OLDPWD
void ft_export(t_task *task, t_environ *environ)
{
	char *key;
	char *value;
	int i;

	i = 1; //1 porque argv[0] contiene el nombre del comando y solo queremos los argumentos
	while(task->argv[i])
	{
		key = getkey(task->argv[i]);
		if (!validate_key(key))
		{
			export_error(task->argv[i], environ);
			free(key);
			i++;
			continue;
		}
		value = getvalue(task->argv[i]);
			if(search_var(environ->envp, key) == -1)
				add_var(key, value, environ);
			else
				change_var(key, value, environ);
		free(key);
		free(value);
		i++;
	}	

}

int non_pipable_builtin(t_tree *tree, t_environ *environ)
{
	t_task *task;

	if(tree->type == TASK)
	{
		task = (t_task *)tree;
		if(!((t_task *)tree)->cmd)
			return (ALL_OK);
		//CD NO ESTA TESTEADO, abstraer funcion cd
		if(!ft_strcmp(task->cmd, "cd"))
		{	
			if(countargs((t_task *)tree) != 2)
				ft_putstr_fd("minishell: cd: too many arguments", 2);
			else if(chdir(((t_task *)tree)->argv[1]))
				perror("minishell: cd:");
			//pendiente: gestionar error
			//gestionar $?
			//pendiente: actualizar variable PWD y OLDPWD(oculta)
			return (CONTINUE);
		}
		else if(!ft_strcmp(((t_task *)tree)->cmd, "exit"))
		{
			//free_tree(tree);
			return(FINISH);
		}
		else if(!ft_strcmp(((t_task *)tree)->cmd, "export"))
		{
			ft_export(task, environ);
			return (CONTINUE);
		}
		else if(!ft_strcmp(((t_task *)tree)->cmd, "unset"))
		{
			//TODO
			printf("unset no esta implementado\n");
		}
	}
	return (ALL_OK);
}