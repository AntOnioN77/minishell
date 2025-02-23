#include "../minishell.h"
#include "../libft/headers/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//newsize debe ser siempre superior a oldsize
void *custom_realloc(void **pnt, size_t oldsize, size_t newsize)
{
	void *newpnt;

	if (*pnt == NULL)
		return (NULL);
	if(oldsize >= newsize)
		return (*pnt);
	newpnt = ft_calloc(1, newsize);
	if(newpnt == NULL)
		return(NULL);
	ft_memcpy(newpnt, *pnt, oldsize);
	free(*pnt);
	*pnt = newpnt;
	return (newpnt);
}


e_errors add_var(char *key, char *value, t_environ *environ)
{
	size_t len;
	char *newvar;

	if (key == NULL || value == NULL || key[0] == '\0')
		return(1);//asignar un valor mas significativo para este error
	if (environ->next >= environ->alloced)
	{
		if(!custom_realloc((void **)&(environ->envp), environ->alloced * sizeof(char *), environ->alloced * 2 * sizeof(char *)))
			return(errno);
		environ->alloced = environ->alloced * 2;
	}
	len = ft_strlen(key) + ft_strlen(value) + 2;// +1 caracter nulo final, +1 signo "=" clave=valor
	newvar = ft_calloc(len, sizeof(char));
	if (newvar == NULL)
		return (errno);
	ft_strlcpy(newvar, key, len);
	ft_strlcat(newvar, "=", len);
	ft_strlcat(newvar, value, len);
	environ->envp[environ->next] = newvar;
	environ->next=environ->next + 1;
	return(0);
}


// Función para imprimir el entorno
void print_env(t_environ *environ)
{
	int i;
	
	i = 0;
	while(i < environ->next)
	{
		printf("%s\n", environ->envp[i]);
		i++;
	}
}

// Test Suite
void run_tests()
{
	t_environ env;
	env.envp = ft_calloc(4, sizeof(char *));
	env.next = 0;
	env.alloced = 4;

	printf("=== TEST 1: Agregar primera variable ===\n");
	add_var("USER", "john", &env);
	print_env(&env);
	printf("\n");

	printf("=== TEST 2: Agregar múltiples variables sin realloc ===\n");
	add_var("SHELL", "/bin/bash", &env);
	add_var("HOME", "/home/john", &env);
	print_env(&env);
	printf("\n");

	printf("=== TEST 3: Agregar más variables para forzar realloc ===\n");
	add_var("PATH", "/usr/bin:/bin", &env);
	add_var("EDITOR", "vim", &env);
	print_env(&env);
	printf("\n");

	printf("=== TEST 4: Agregar variable con clave vacía ===\n");
	printf("Resultado: %d\n\n", add_var("", "value", &env));

	printf("=== TEST 5: Agregar variable con valor vacío ===\n");
	printf("Resultado: %d\n\n", add_var("EMPTY", "", &env));
	print_env(&env);

	printf("=== TEST 6: Agregar variable con NULL en clave ===\n");
	printf("Resultado: %d\n\n", add_var(NULL, "value", &env));

	printf("=== TEST 7: Agregar variable con NULL en valor ===\n");
	printf("Resultado: %d\n\n", add_var("KEY", NULL, &env));

	// Liberar memoria
	for (int i = 0; i < env.next; i++)
		free(env.envp[i]);
	free(env.envp);
}

int main()
{
	run_tests();
	return 0;
}