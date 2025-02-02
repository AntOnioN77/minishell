#include "minishell.h"
#include "executor.h"

int	error_exe(int er, char *cmd)
{
	char	*msg;

	msg = NULL;
	//De momento no hay error 0, 1, 2, 3, 4, 5 y más
	if (er == 0)
		msg = ft_strjoin("Error: Need 4 arguments:\n", cmd);
	else if (er == 1)
		ft_putstr_fd("Error: Pipe not open\n", 2);
	else if (er == 2)
		ft_putstr_fd("Error: PID incorrect\n", 2);
	else if (er == 3)
		msg = ft_strjoin("Error: No such file or directory: ", cmd);
	else if (er == 4)
		msg = ft_strjoin("Error: Permission denied: ", cmd);
	else if (er == 5)
		msg = ft_strjoin("Error. Command not found: ", cmd);
	else if (er > 5)
		perror("Error");
	if (msg)
	{
		ft_putstr_fd(msg, 2);
		free (msg);
	}
	return (1);
}