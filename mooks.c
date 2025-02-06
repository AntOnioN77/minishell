/*                        ███╗   ███╗ ██████╗  ██████╗ ██╗  ██╗███████╗                                  
                        ████╗ ████║██╔═══██╗██╔═══██╗██║ ██╔╝██╔════╝                                  
                        ██╔████╔██║██║   ██║██║   ██║█████╔╝ ███████╗                                  
                        ██║╚██╔╝██║██║   ██║██║   ██║██╔═██╗ ╚════██║                                  
                        ██║ ╚═╝ ██║╚██████╔╝╚██████╔╝██║  ██╗███████║                                  
                        ╚═╝     ╚═╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═╝╚══════╝                                  
                                                                                                       
███████╗██╗   ██╗███╗   ██╗ ██████╗██╗ ██████╗ ███╗   ██╗███████╗███████╗     ██████╗ ██╗   ██╗███████╗
██╔════╝██║   ██║████╗  ██║██╔════╝██║██╔═══██╗████╗  ██║██╔════╝██╔════╝    ██╔═══██╗██║   ██║██╔════╝
█████╗  ██║   ██║██╔██╗ ██║██║     ██║██║   ██║██╔██╗ ██║█████╗  ███████╗    ██║   ██║██║   ██║█████╗  
██╔══╝  ██║   ██║██║╚██╗██║██║     ██║██║   ██║██║╚██╗██║██╔══╝  ╚════██║    ██║▄▄ ██║██║   ██║██╔══╝  
██║     ╚██████╔╝██║ ╚████║╚██████╗██║╚██████╔╝██║ ╚████║███████╗███████║    ╚██████╔╝╚██████╔╝███████╗
╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝
                                                                                                       
        ██████╗ ███████╗██████╗ ███████╗███╗   ██╗    ███████╗███████╗██████╗                          
        ██╔══██╗██╔════╝██╔══██╗██╔════╝████╗  ██║    ██╔════╝██╔════╝██╔══██╗                         
        ██║  ██║█████╗  ██████╔╝█████╗  ██╔██╗ ██║    ███████╗█████╗  ██████╔╝                         
        ██║  ██║██╔══╝  ██╔══██╗██╔══╝  ██║╚██╗██║    ╚════██║██╔══╝  ██╔══██╗                         
        ██████╔╝███████╗██████╔╝███████╗██║ ╚████║    ███████║███████╗██║  ██║                         
        ╚═════╝ ╚══════╝╚═════╝ ╚══════╝╚═╝  ╚═══╝    ╚══════╝╚══════╝╚═╝  ╚═╝                         
                                                                                                       
███████╗██╗   ██╗███████╗████████╗██╗████████╗██╗   ██╗██╗██████╗  █████╗ ███████╗                     
██╔════╝██║   ██║██╔════╝╚══██╔══╝██║╚══██╔══╝██║   ██║██║██╔══██╗██╔══██╗██╔════╝                     
███████╗██║   ██║███████╗   ██║   ██║   ██║   ██║   ██║██║██║  ██║███████║███████╗                     
╚════██║██║   ██║╚════██║   ██║   ██║   ██║   ██║   ██║██║██║  ██║██╔══██║╚════██║                     
███████║╚██████╔╝███████║   ██║   ██║   ██║   ╚██████╔╝██║██████╔╝██║  ██║███████║                     
╚══════╝ ╚═════╝ ╚══════╝   ╚═╝   ╚═╝   ╚═╝    ╚═════╝ ╚═╝╚═════╝ ╚═╝  ╚═╝╚══════╝                    */

#include "minishell.h"

int	search_var(char **envp, const char* var)
{
	int pos;

	pos = 0;
	while (envp[pos])
	{
		if (ft_strncmp(envp[pos], var, ft_strlen(var)) == 0)
		{
			return (pos);
		}
		pos++;
	}
	return (-1);
}

char *ft_getenv(const char *name, char *envp[])
{
	int pos;
	char *word_start;

	pos = search_var(envp, name);
	if (pos == -1)
		return(NULL);
	word_start = ft_strchr(envp[pos], '=') + 1;
	return(word_start);
}

/* 
// parse_test requiere este mook:
char *ft_getenv(const char *name, char *envp[])
{
	if (!envp)
		return NULL;
    if (strcmp(name, "VAR") == 0)
		return "value";
    if (strcmp(name, "VAR2") == 0)
		return "value2";
    if (strcmp(name, "EMPTY") == 0)
		return "";
    if (strcmp(name, "LONG") == 0)
		return "_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value_this_is_a_very_long_value";
    if (strcmp(name, "SPACE") == 0)
		return "value with spaces";
    if (strcmp(name, "QUOTES") == 0)
		return "value'with'quotes";
    return NULL;
}
*/

//SOLO PARA PRUEBAS no requiere implementacion
//-Imprime el arbol
void print_tree(t_tree *node, int depth)
{
    if (!node)
        return;
    for (int i = 0; i < depth; i++)
        printf("  ");
    if(node->type == PIPE)
	{
    	printf("PIPE\n");
        print_tree(((t_tree *)((t_pipe *)node)->left), depth + 1);
        print_tree(((t_pipe *)node)->rigth, depth + 1);
	}
    else if(node->type == TASK)
    {
            printf("TASK->cmd: %s\n", ((t_task *)node)->cmd);
			for (int j = 0; ((t_task *)node)->argv[j]; j++)
				printf("\t->argv[%d]: %s\n", j, ((t_task *)node)->argv[j]);
			printf("\t->redir->insymbol: %d\n", ((t_task *)node)->redir.insymbol);
			printf("\t->redir->infoo: %s\n", ((t_task *)node)->redir.infoo);
			printf("\t->redir->outsymbol: %d\n", ((t_task *)node)->redir.outsymbol);
			printf("\t->redir->outfile: %s\n", ((t_task *)node)->redir.outfile);
    }
	else
		return; //llegar aqui indicaria un error
}
