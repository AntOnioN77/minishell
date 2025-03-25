/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antofern <antofern@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:58:52 by antofern          #+#    #+#             */
/*   Updated: 2025/03/25 12:58:54 by antofern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/headers/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include "enums.h"
# include "structs.h"

# define WHITESPACES " \r\n\v\t"
# define DELIMITERS "|<> \r\n\v\t"
# define S_LINE_MAX 4096
# define IS_SIGNAL 128
# define BUFFER_SIZE 1024

//From: #include <linux/limits.h>
# define PATH_MAX 4096
# define NAME_MAX 255

//VARIABLE GLOBAL
extern int	g_ctrlc;

/*****************************************************************************/
/* 								PROTOTYPES									 */
/*****************************************************************************/
/*_____________________Parser_Prototypes______________________________________*/
//Location: main.c
void		status_control(int status, t_environ *environ);
void		shell_cycle(t_tree *tree, t_environ *environ);
void		print_error(char *cmd, char *error_msg);
//Location: parser/constructors.c
t_task		*createtask(char *segment, char *end);
t_tree		*createpipe(char *line, char *pnt);
//Location parser/expansor.c
t_errors	expandstr(char **origin, t_garbage *garbage, char *envp[]);
t_errors	touch_up_tree(t_tree *node, char *envp[]);
t_errors	expand_task(t_task *node, char *envp[]);
//Location: parser/expansor_utils.c
int			is_expansible(char *str);
int			count_expansions(t_task *node);
int			calculate_expansion_length(char *str, char *envp[]);
//Location: parser/free_tree.c
void		free_tree(t_tree *node);
//Location: parser/get_redir.c
void		get_redir(char **segment, char *end, t_redir *redir);
//Location: parser/build_tree.c
void		getpntword(char **segment, char *end, char **dst);
int			count_cmdflags(char *segment, char *end);
int			parse_task(char *segment, char *end, t_task *task);
int			parsepipe(char *line, t_tree **ret);
t_tree		*build_tree(char *line);
//Location: parser/str_utils.c
int			isdelimiter(char c);
void		nullify_delimiters(char *str);
void		skipwhitesp(char **segment, char *end);
int			skip_quotes(char **strpnt, char *end);
int			strnchr_outquot(char **str, char *end, char c);
//Location: mooks.c
char		*ft_getenv(const char *name, char *envp[]);
void		print_tree(t_tree *node, int depth);
//LOCATION: parser/check_tree.c
int			check_tree(t_tree *tree, char **envp);
//LOCATION: create_heredoc.c
t_errors	create_heredoc(t_redir *redir);
char		*get_tmp_name(t_errors *error);
t_errors	heredoc_writer(char *separator, t_redir *redir);
/*static t_errors write_heredoc_fork(int fd, char *separator, size_t seplen);
static t_errors write_heredoc_line(int fd, char *separator, size_t seplen);*/
//LOCATION: signal.c
void		handle_sigint(int signal);
void		handle_sigint_heredoc(int signal);
void		signal_conf(void);
//LOCATION: handler_error.c
void		print_error(char *cmd, char *error_msg);
void		ft_perror(int error);
t_errors	error_var(t_errors error, t_environ *environ);
t_errors	handlerr(t_errors error, t_tree **tree, t_environ *environ);
//LOCATION
void		free_task(t_tree *node);
void		free_tree(t_tree *node);
//LOCATION
t_errors	continue_cmd_tree(t_tree **tree, char **envp);
t_errors	get_cmd_tree(t_tree **tree, t_environ *environ);
//LOCATION parser/1_expansor/expand_one.c
int			expand_one(char **newline, char *buffer, char *marker, char **envp);
t_errors	join_line_buffer(char **newline, char *buffer);
//LOCATION parser/2_build_tree/files.c
char		*findchars(char *str, char *end, char *wanted);
void		check_file(char *segment, char *end, t_redir *redir);
void		create_file(char *segment, char *end, int flag, t_redir *redir);

/*_____________________Environment_Prototypes_________________________________*/
//LOCATIONS: create_envp.c
int			count_to_null(void **pnt);
t_errors	copy_prev_envp(char **original, t_environ *environ);
t_errors	create_envp(char **original, t_environ *environ);
void		print_env(t_environ *environ);
//LOCATION: init_envp.c
t_errors	set_shlvl(t_environ *environ);
t_errors	set_pathshell(t_environ *environ, char *path, t_errors error);
t_errors	init_envp(t_environ *environ);
//LOCATION: get_envp.c
char		*ft_getenv(const char *name, char *envp[]);
//LOCATION: var_operation.c
void		*custom_realloc(void **pnt, size_t oldsize, size_t newsize);
char		*var_buid(char *key, char *newvalue);
t_errors	change_var(char *key, char *newvalue, t_environ *environ);
t_errors	add_var(char *key, char *value, t_environ *environ);
int			search_var(char **envp, const char *var);

/*_____________________Builtins Prototypes____________________________________*/
//LOCATIONS: builtins.c
t_errors	ft_echo(t_task *task);
void		ft_env(t_environ *environ);
int			is_builtin(char *cmd);
t_errors	builtins_exe(t_task *task, t_environ *environ);
//LOCATIONS: ft_cd.c
void		ft_cd(t_task *task, t_environ *environ);
int			cd_destination(t_task *task, char **dest, t_environ *environ);
//LOCATIONS: ft_exit.c
void		ft_exit(t_task *task, t_tree *tree, t_environ *environ);
int			select_exitcode(t_task *task, t_environ *environ, char *argument);
//LOCATIONS: ft_export.c
int			validate_key(char *key);
void		ft_export(t_task *task, t_environ *environ);
char		*getvalue(char *var);
void		export_error(char *identifier, t_environ *environ);
//LOCATIONS: ft_unset.c
void		ft_unset(char **argv, t_environ *environ);
void		ft_unset_one(char *key, t_environ *environ, int index);
//LOCATIONS: non_pipable_builtin.c
int			countargs(t_task *task);
int			non_pipable_builtin(t_tree *tree, t_environ *environ);

/*_____________________Executor Prototypes____________________________________*/
//LOCATIONS: process.c
int			wait_all(t_tree *node);
//LOCATIONS: apply_redirs.c
t_errors	file_redirector(int newfd, char *file, int opflag, char **wordfail);
//LOCATIONS: path.c
char		*com_path(char *cmd, char **envp, t_errors *err);
//LOCATIONS: executor.c
t_errors	exec_pipe(t_pipe *pipe_node, t_environ *environ, int in);
//LOCATIONS: child.c
t_errors	child(t_task *task, t_environ *environ, int in, int out);

/*______________________________Others_Prototypes_________________________*/
t_errors	expansor(char **line, char **envp);
int			search_var(char **envp, const char *var);
t_errors	change_var(char *key, char *newvalue, t_environ *environ);
t_errors	add_var(char *key, char *value, t_environ *environ);
char		*getkey(char *var);
void		unquote(char *str);
void		free_arr(char **s);
void		handle_sigint_vis(int signal);
int			is_quoted(char *str, int original_flag);
int			countargs(t_task *task);
void		unquote_task(t_task *node);

#endif
