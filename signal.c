
#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

//volatile sig_atomic_t g_ctrlc = 0;

void handle_sigint_heredoc2(int signal)
{
    g_ctrlc = signal;
    //rl_replace_line("", 0);  // Reemplaza la línea actual con una cadena vacía
    rl_done = 1;             // Indica que la lectura ha terminado
	//kill(getpid(), SIGTERM);
	rl_redisplay();
}

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
	//rl_replace_line("^C", 2);
	rl_on_new_line();
	rl_redisplay();
	rl_done = 1;
	//close(STDIN_FILENO);
}

/**
 * Configura las señales para CTRL+C (SIGINT) y CTRL+\ (SIGQUIT)
 */
void	signal_conf(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}