#include "../minishell.h"
#include "../libft/headers/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

e_errors change_var(char *key, char *newvalue, t_environ *environ)
{
	size_t len;
	size_t keylen;
	char *newvar;
	int i;
	char **envp;

	if (!key || !newvalue || !environ)
		return (1);//poco expresivo
	envp = environ->envp;
	keylen= ft_strlen(key);
	len = keylen + ft_strlen(newvalue) + 2;// +1 caracter nulo final, +1 signo "=" clave=valor
	newvar = ft_calloc(len, sizeof(char));
	if (newvar == NULL)
		return (errno);
	ft_strlcpy(newvar, key, len);
	ft_strlcat(newvar, "=", len);
	ft_strlcat(newvar, newvalue, len);
	i = 0;
	while(i < environ->next)
	{
		if (ft_strncmp(envp[i], key, keylen) == 0)
		{
			free(envp[i]);
			envp[i] = newvar;
			return(0);
		}
		i++;
	}
	free(newvar);
	return(1);

}


// Función para imprimir el entorno
void print_env(t_environ *environ)
{
	printf("=== ENV ===\n");
	for (int i = 0; i < environ->next; i++)
		printf("%s\n", environ->envp[i]);
	printf("===========\n\n");
}

// Test Suite
void run_tests()
{
	t_environ env;
	env.envp = ft_calloc(4, sizeof(char *));
	env.next = 3;
	env.alloced = 4;

	// Inicializar entorno
	env.envp[0] = strdup("USER=john");
	env.envp[1] = strdup("SHELL=/bin/bash");
	env.envp[2] = strdup("PATH=/usr/bin:/bin");

	print_env(&env);

	printf("=== TEST 1: Modificar variable existente ===\n");
	printf("Resultado: %d\n", change_var("USER", "doe", &env));
	print_env(&env);

	printf("=== TEST 2: Intentar modificar una variable que no existe ===\n");
	printf("Resultado: %d\n", change_var("EDITOR", "vim", &env));
	print_env(&env);

	printf("=== TEST 3: Modificar la primera variable del entorno ===\n");
	printf("Resultado: %d\n", change_var("USER", "alice", &env));
	print_env(&env);

	printf("=== TEST 4: Modificar la última variable del entorno ===\n");
	printf("Resultado: %d\n", change_var("PATH", "/sbin", &env));
	print_env(&env);

	printf("=== TEST 5: Modificar una variable con valor vacío ===\n");
	printf("Resultado: %d\n", change_var("SHELL", "", &env));
	print_env(&env);

	printf("=== TEST 6: Intentar modificar con key == NULL ===\n");
	printf("Resultado: %d\n\n", change_var(NULL, "value", &env));

	printf("=== TEST 7: Intentar modificar con newvalue == NULL ===\n");
	printf("Resultado: %d\n\n", change_var("USER", NULL, &env));

	printf("=== TEST 8: Intentar modificar con environ == NULL ===\n");
	printf("Resultado: %d\n\n", change_var("USER", "doe", NULL));

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