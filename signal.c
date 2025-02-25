
#include "minishell.h"

/**
 * Imprime una nueva línea y redibuja el prompt de readline
 */
void	handle_sigint(int signal)
{
	g_ctrlc = signal;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Gestiona Ctrl+C con heredoc
 */
void	handle_sigint_heredoc(int signal)
{
	g_ctrlc = signal;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	//rl_done = 1;
	//close(STDOUT_FILENO);
}

/**
 * Configura las señales para CTRL-C (SIGINT) y CTRL-\ (SIGQUIT)
 */
void	signal_conf(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
