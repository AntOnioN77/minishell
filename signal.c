
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include "executor.h"

/**
 * Imprime una nueva línea y redibuja el prompt de readlinewwwwwwwwwwwwwwwwwwwwwwwwwww
 */
void	handle_sigint(int signal)
{
	g_ctrlc = signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * Gestiona Ctrl+C con heredoc
 */
void	handle_sigint_heredoc(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	close_fds(3);
	exit(E_SIGINT);
}

/**
 * Configura las señales para CTRL+C (SIGINT) y CTRL+\ (SIGQUIT)
 */
void	signal_conf(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
