
#include "minishell.h"


//retorna 1 si key es valido (el primer caracter es alfabetico, contiene solo caracteres alphanumericos y no es una cadena vacía)
int	validate_key(char *key)
{
	int	i;

	if (!ft_strcmp(key, "") || !ft_strcmp(key, "PWD")
		|| !ft_strcmp(key, "SHLVL") || !ft_strcmp(key, "SHELL")
		|| !ft_strcmp(key, "HOME") || !ft_strcmp(key, "OLDPWD"))
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	export_error(char *identifier, t_environ *environ)
{
	char	*atomic;
	char	*aux;

	aux = ft_strjoin("mini$hell: export: '", identifier);
	atomic = ft_strjoin(aux, "': not a valid identifier\n");
	free(aux);
	ft_putstr_fd(atomic, 2);
	free(atomic);
	change_var("?", "1", environ);
}

char	*getvalue(char *var)
{
	char	*start;

	start = ft_strchr(var, '=');
	if (!start)
		return (NULL);
	start++;
	return (ft_strdup(start));
}

//export no debe cambiar variables como $? $PWD o $OLDPWD
void	ft_export(t_task *task, t_environ *environ)
{
	char	*key;
	char	*value;
	int		i;

	i = 1; //1 porque argv[0] contiene el nombre del comando y solo queremos los argumentos
	change_var("?", "0", environ);
	while (task->argv[i])
	{
		key = getkey(task->argv[i]);
		if (!validate_key(key))
		{
			export_error(task->argv[i], environ);
			free(key);
			i++;
			continue ;
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
