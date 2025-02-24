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
#include <sys/stat.h>

void test_fds(char *where)
{
    char buffer[1024] = {0};  // Buffer para almacenar todo el output
    unsigned long offset = 0;           // Para trackear la posición en el buffer
    int i = 0;
    struct stat statbuf;

    // Construir el encabezado
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, 
        "___TEST_FD__\n   %s proceso:%d\n", where, getpid());

    // Construir la información de los file descriptors
    while (i < 20 && offset < sizeof(buffer))
    {
        if (fstat(i, &statbuf) == -1)
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, 
                "| fd[%d] 🔴 | ", i);
        else
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, 
                "| fd[%d] 🟢 | ", i);
        
        if (i % 5 == 0)
            offset += snprintf(buffer + offset, sizeof(buffer) - offset, "\n");
        i++;
    }
	offset += snprintf(buffer + offset, sizeof(buffer) - offset, "\n");
    // Imprimir todo de una vez
    write(STDERR_FILENO, buffer, offset);
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
			printf("\t->redir->tmp_file: %s\n", ((t_task *)node)->redir.tmp_file);	
			printf("\t->redir->error: %d\n", ((t_task *)node)->redir.error);
			printf("\t->redir->outsymbol: %d\n", ((t_task *)node)->redir.outsymbol);
			printf("\t->redir->outfile: %s\n", ((t_task *)node)->redir.outfile);
    }
	else
		return; //llegar aqui indicaria un error
}
