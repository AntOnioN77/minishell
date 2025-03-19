//Hay buitins que en el contexto de minishell no tiene sentido que formen parte de un pipe
//Son: cd, export, unset y exit.
//la solucion mas directa es ejecutarlos solo cuando el arbol sea unicamente de un nodo

#include "minishell.h"
#include <linux/limits.h>

int countargs(t_task *task)
{
	int i;

	if (!task || !task->argv)
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
		return (NULL);
	start++;
	return (ft_strdup(start));
	
}

void export_error(char* identifier, t_environ *environ)
{
	char *atomic;
	char *aux;

	aux = ft_strjoin("mini$hell: export: '", identifier);
	atomic = ft_strjoin(aux, "': not a valid identifier\n");
	free(aux);
	ft_putstr_fd(atomic, 2);
	free(atomic);
	change_var("?", "1", environ);
}

//retorna 1 si key es valido (el primer caracter es alfabetico, contiene solo caracteres alphanumericos y no es una cadena vacía)
int validate_key(char *key)
{
	int i;
	if (!ft_strcmp(key, "") || !ft_strcmp(key,"PWD") || !ft_strcmp(key,"SHLVL")
		|| !ft_strcmp(key,"SHELL")|| !ft_strcmp(key,"HOME")|| !ft_strcmp(key,"OLDPWD"))
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
			return (0);
	i =1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_') 
			return (0);
		i++;
	}
	return (1);
}

void ft_unset_one(char *key, t_environ *environ)
{
	int index;
	int i;

	index = search_var(environ->envp, key);
	if (index == -1)
		return;

	if (!ft_strcmp(key,"PWD") || !ft_strcmp(key,"SHLVL")
		|| !ft_strcmp(key,"SHELL")|| !ft_strcmp(key,"HOME")|| !ft_strcmp(key,"OLDPWD")|| !ft_strcmp(key,"?"))
	{
		ft_putstr_fd(" PWD, SHLVL, SHELL, HOME, OLDPWD, ?, are internal variables, not unsettable.\n", 2);
		return;
	}
	free(environ->envp[index]);
	i = 1;
	while(environ->envp[index + i])
	{
		environ->envp[index + i - 1] =  environ->envp[index + i];
		i++;
	}
	environ->envp[index + i -1] = NULL;
	environ->next = environ->next - 1;
}
void ft_unset(char **argv, t_environ *environ)
{
	int i;

	change_var("?", "0", environ);
	if (!argv || !*argv)
		return;
	i = 1; 	
	while(argv[i])
	{
		ft_unset_one(argv[i], environ);
		i++;
	}
}

//export no debe cambiar variables como $? $PWD o $OLDPWD
void ft_export(t_task *task, t_environ *environ)
{
	char *key;
	char *value;
	int i;

	i = 1; //1 porque argv[0] contiene el nombre del comando y solo queremos los argumentos
	change_var("?", "0", environ);
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
			if (search_var(environ->envp, key) == -1)
				add_var(key, value, environ);
			else
				change_var(key, value, environ);
		free(key);
		free(value);
		i++;
	}	

}

//libera todo y termina minishell con exit(), solo retorna en caso de error
void ft_exit(t_task *task, t_tree *tree, t_environ *environ)
{
	int	exitcode;
	unsigned char truncated;
	char *argument;

	if (countargs(task) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		change_var("?", "1", environ);
		return;
	}
	if (countargs(task) == 2)
	{
		argument = task->argv[1];
		skipwhitesp(&argument, argument + ft_strlen(argument));
		if (*argument == '+' || *argument == '-')
			argument++;
		while (ft_isdigit(*argument))
			argument++;
		if (*argument != '\0')
		{
			ft_putstr_fd("bash: exit: 1 0: numeric argument required\n", 2);
			exitcode = 2;
		}
		else
			exitcode = ft_atoi(task->argv[1]);
	}
	else
		exitcode = ft_atoi(ft_getenv("?", environ->envp));

	truncated = (unsigned char) exitcode;
	free_tree(tree);
	free_arr(environ->envp);
	exit(truncated);
}

int	non_pipable_builtin(t_tree *tree, t_environ *environ)
{
	t_task	*task;

	if (tree->type == TASK)
	{
		task = (t_task *)tree;
		if (!((t_task *)tree)->cmd)
			return (ALL_OK); 
		if (!ft_strcmp(task->cmd, "cd"))
		{	
			ft_cd(task, environ);
		}
		else if (!ft_strcmp(task->cmd, "exit"))
		{
			ft_exit(task, tree, environ);
		}
		else if (!ft_strcmp(task->cmd, "export"))
		{
			ft_export(task, environ);
		}
		else if (!ft_strcmp(task->cmd, "unset"))
			ft_unset(task->argv, environ);
		else		//si se cumple alguna de las condiciones retornamos continue, si la task no era un builtin retornamos all ok, para no interrumpir el flujo normal
			return (ALL_OK);
		return (CONTINUE);
	}
	return (ALL_OK);
}
