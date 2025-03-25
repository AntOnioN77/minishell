/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fibo <fibo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 21:14:20 by jperez-r          #+#    #+#             */
/*   Updated: 2025/03/25 02:49:07 by fibo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <sys/stat.h>
#include <stdio.h>

static enum e_err	child_error_handler(enum e_err err, char *cmd)
{
	if (err != 0)
	{
		if (cmd == NULL)
			return (child_error_handler_fail);
		if (err == COM_NOT_FOUND)
			print_error(cmd, ": Command not found\n");
		else if (err == IS_A_DIR)
			print_error(cmd, ": Is a directory\n");
		else if (err == NO_PERMISSION)
		{
			err = 126;
			print_error(cmd, ": Permission denied\n");
		}
		else if (err == NO_EXIST)
		{
			err = 127;
			print_error(cmd, ": No such file or directory\n");
		}
		else
			perror("minishell");
		close_fds(0);
		return (err);
	}
	return (0);
}

enum e_err	repipe_child(t_task *task, int in, int out, char **word_fail)
{
	enum e_err	err;

	if (out != STDOUT_FILENO)
	{
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if (in != STDIN_FILENO)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	close(3);
	close(4);
	err = apply_redirs(&(task->redir), word_fail);
	if (err != 0)
		return (err);
	return (0);
}

enum e_err	child(t_task *task, t_environ *environ, int in, int out)
{
	enum e_err	err;
	char		*pathcmd;
	char		*word_fail;
	char		**envp;

	envp = environ->envp;
	word_fail = NULL;
	signal(SIGINT, SIG_DFL);
	err = repipe_child(task, in, out, &word_fail);
	if (child_error_handler(err, word_fail))
		return (1);
	if (is_builtin(task->cmd))
		return (builtins_exe(task, environ));
	pathcmd = com_path(task->cmd, envp, &err);
	if (err)
		return (child_error_handler(err, task->cmd));
	rl_clear_history();
	execve(pathcmd, task->argv, envp);
	err = errno;
	free(pathcmd);
	return (err);
}

enum e_err	create_child(t_task *task, t_environ *environ, int in, int out)
{
	int			pid;
	enum e_err	err;

	err = 0;
	pid = fork();
	if (pid == -1)
		return (errno);
	task->pid = pid;
	if (pid == 0)
	{
		err = child(task, environ, in, out);
		if (err)
			return (err);
	}
	else
		signal(SIGINT, SIG_IGN);
	if (out != STDOUT_FILENO)
		close(out);
	if (in != STDIN_FILENO)
		close(in);
	return (err);
}
