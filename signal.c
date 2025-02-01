
#include "minishell.h"

/**
 * Imprime una nueva línea y redibuja el prompt de readline
 */
void handle_sigint(int signal)
{
	if (signal == SIGINT) {
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
    }
}
void handle_sigquit(int signal)
{
	(void)signal;
    rl_on_new_line();
    rl_replace_line(rl_line_buffer, 0);
    rl_redisplay();
}

/**
 * Configura las señales para CTRL-C (SIGINT) y CTRL-\ (SIGQUIT)
 */
void	signalConf(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	
	//Esto funciona para silenciar completamente Ctrl-\, pero está fuera de norma
	/*sigset_t block;
    sigemptyset(&block);
    sigaddset(&block, SIGQUIT);
    sigprocmask(SIG_BLOCK, &block, NULL);*/
}