#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **envp)
{
	int i=0;
	int envp_count = 0;
	
	if(fork() == 0)
	{
		char **temp_envp;
		while (envp[envp_count])
		{
			envp_count++;
        }
		i=0;
		temp_envp = malloc((envp_count + 2) * sizeof(char *));
		while(envp[i])
		{
			temp_envp[i] = envp[i];
			i++;
		}
		temp_envp[i] = "NEW_VAR=constante de prueba";
		temp_envp[i + 1] = NULL;
		//free(envp);
		envp = temp_envp;
		i=0;
		printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>child:\n");
		while(envp[i])
		{
			printf("%s\n", envp[i]);
			i++;
		}
		free(temp_envp);
		exit(0);
	}
	wait(NULL);
	printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<parent:\n");
	while(envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}

}