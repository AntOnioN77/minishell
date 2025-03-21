

#ifndef MINISHELL_H
# define MINISHELL_H


# include "libft/headers/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>

///////revisar
# include <limits.h>
/////////

# define WHITESPACES " \r\n\v\t"
# define DELIMITERS "|<> \r\n\v\t"
# define S_LINE_MAX 4096
# define IS_SIGNAL 128
//VARIABLE GLOBAL
//static int	g_ctrlc;
extern int	g_ctrlc;

/*****************************************************************************/
/* 								ENUMS										 */
/*****************************************************************************/

typedef enum e_nodes
{
	NONE,
	LINE,//probablemente innecesario
	PIPE,
	REDIR,//probablemente innecesario
	TASK,
	SYNTAX
}	e_nodes;

typedef enum e_symbols
{
	none,
	infile,//<
	outfile,//>
	heredoc,//<<
	append//>>
} e_symbols;



typedef enum e_errors
{
	ALL_OK,
	IS_A_DIR = 126,
	COM_NOT_FOUND = 127,
	E_SIGINT = 130,
	E_SIGQUIT = 131,
	ERROR_MALLOC = 151, // evitamos solapar valores de errno, de esta manera
	//nuestras funciones pueden retornar tanto valores capturados de errno, como
	//nuestros propios casos de error
	READ_SIGINT,
	NO_PERMISSION,
	NO_EXIST,
	INVALID_TYPE,//solo para debug
	TASK_IS_VOID,
	SYNTAX_ERROR,
	READLINE_FAIL,//lo he cambiado por retornar errno (14feb)
	TMP_FILE_ERROR,
	LINE_TOO_LONG,
	child_error_handler_fail,   //SOLO ODEBUG BORRAR!!!!!!!!!!!!!!!!!
	FAIL_BUILTINS_EXE,			//SOLO ODEBUG BORRAR!!!!!!!!!!!!!!!!!
	CONTINUE,
	END_CYCLE,
	FINISH
} e_errors;

/*****************************************************************************/
/* 								STRUCTS										 */
/*****************************************************************************/

/*_________________________TREE_STRUCTS______________________________________*/

/*Este struct no tiene un caso de uso real, funciona como interface. Los nodos del arbol
son en realidad t_line t_pipe t_redir t_task.
El uso de una interface nos permite pasar cualquiera de estos tipos como argumento a una
función. El elemento type, permite a esa función determinar que tipo de dato ha recibido
en realidad.*/
typedef struct s_tree
{
	e_nodes	type;
	char	*line;		//solo el primer nodo del arbol contendrá line!=NULL
	char	*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
}	t_tree;


/*La linea recibida como task se parsea de izquuierda a derecha, sucesivas redirecciones
se sobreescriben. de manera que hay un solo t_redir por cada t_task.
si no se encuentra ninguna redireccion los e_symbols pemanecen "none"
*/
typedef struct s_redir
{
	e_nodes		type;
	e_symbols	insymbol;//< o <<
	char		*infoo;// será un archivo para <, O un separator para <<
	e_symbols	outsymbol;//>> o >
	char		*outfile;
	char		*tmp_file;//Es creado en caso de insymbol << por la funcion heredoc_handler requiere unlink, y despues free
	e_errors	error; //Error causado durante la creación de tmp_file
}	t_redir;

typedef struct s_garbage {
	void	**pointers;
	int		size;
	int		current;
}	t_garbage;

typedef struct s_task
{
	e_nodes		type;
	char		*line;		//solo el primer nodo del arbol contendrá line!=NULL
	char		*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
	t_redir		redir;
	char		*cmd;
	char		**argv;
	t_garbage	garb;
	int			pid;
}	t_task;

//si left o rigth fuesen NULL indica error de reserva de memoria, hay que liverar el arbol entero y lanzar error.
//Un pipe tal que "ls|   (nada)" generaria: pipe->rigth->task, y task tendria sus elementos vacios lo cual no es un error, este ejemplo en concreto deja a bash pendiente de entrada.
//left siempre debe contener un elemento task, y no puede contener otra cosa
//rigth puede contener otro elemento pipe o un elemento task
typedef struct s_pipe {
	e_nodes	type;
	char	*line;		//solo el primer nodo del arbol contendrá line!=NULL
	char	*line_extra;//Solo en caso de "ls|(vacio)" line_extra !=NULL
	t_task	*left;
	t_tree	*rigth;
}	t_pipe;



/*______________________________Other_Structs____________________________*/

typedef struct s_environ {
	// gestion de variables de netorno:
	char	**envp;
	int		next;//posicion no inicializada (null) donde podemos escribir la siguiente variable global
	int		alloced; //numero total de posiciones allocadas, de manera que si next+1 < alloced, no sera necesario reallocar memoria

	// gestion de variables locales:
	char	**local;
	int		locnext;
	int		localloced;

//	sighandler_t *sigint_handler;
}	t_environ;


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
t_tree		*createpipe(char *line,char *pnt);
//Location parser/expansor.c
e_errors	expandstr(char **origin, t_garbage *garbage, char *envp[]); //cuando test-expandstr no sea necesario, hacer esta funcion estatica
e_errors	touch_up_tree(t_tree *node, char *envp[]);
e_errors	expand_task(t_task *node, char *envp[]);
//Location: parser/expansor_utils.c
int			is_expansible(char *str);
int			count_expansions(t_task *node);
int			calculate_expansion_length(char *str, char *envp[]);
int			handle_dollar(char **new_str, char **str, char **marker, char *envp[]);
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
void		print_tree(t_tree *node, int depth); //BORRAR funcion solo para pruebas
//LOCATION: parser/check_tree.c
int			check_tree(t_tree *tree, char **envp);
//LOCATION: create_heredoc.c
e_errors	create_heredoc(t_redir *redir);
char		*get_tmp_name(e_errors *error);
e_errors	heredoc_writer(char *separator, t_redir *redir);
/*static e_errors write_heredoc_fork(int fd, char *separator, size_t seplen);
static e_errors write_heredoc_line(int fd, char *separator, size_t seplen);*/
//LOCATION: signal.c
void		handle_sigint(int signal);
void		handle_sigint_heredoc(int signal);
void		signal_conf(void);
//LOCATION: handler_error.c
void		print_error(char *cmd, char *error_msg);
void		ft_perror(int error);
e_errors	error_var(e_errors error, t_environ *environ);
e_errors	handlerr(e_errors error, t_tree **tree, t_environ *environ);
//LOCATION
void		free_task(t_tree *node);
void		free_tree(t_tree *node);
//LOCATION
e_errors	continue_cmd_tree(t_tree **tree, char **envp);
e_errors	get_cmd_tree(t_tree **tree, t_environ *environ);


/*_____________________Environment_Prototypes_________________________________*/
//LOCATIONS: create_envp.c
int			count_to_null(void **pnt);
e_errors	copy_prev_envp(char **original, t_environ *environ);
e_errors	create_envp(char **original, t_environ *environ);
void test_fds(char *where);
void print_env(t_environ *environ);
//LOCATION: init_envp.c
e_errors	set_shlvl(t_environ *environ);
e_errors	set_pathshell(t_environ *environ, char *path, e_errors error);
e_errors	init_envp(t_environ *environ);
//LOCATION: get_envp.c
char		*ft_getenv(const char *name, char *envp[]);
//LOCATION: var_operation.c
void		*custom_realloc(void **pnt, size_t oldsize, size_t newsize);
char		*var_buid(char *key, char *newvalue);
e_errors 	change_var(char *key, char *newvalue, t_environ *environ);
e_errors	add_var(char *key, char *value, t_environ *environ);
int			search_var(char **envp, const char* var);

/*_____________________Builtins Prototypes____________________________________*/
//LOCATIONS: builtins.c
e_errors	ft_echo(t_task *task);
void		ft_env(t_environ *environ);
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
void		close_fds(int fd_since);
//LOCATIONS: apply_redirs.c
e_errors	file_redirector(int newfd, char *file, int opflag, char **wordfail);
e_errors	apply_redirs(t_redir *redir, char **word_fail);
//LOCATIONS: path.c
char		*com_path(char *cmd, char **envp, e_errors *err);
//LOCATIONS: executor.c



/*______________________________Others_Prototypes_________________________*/
e_errors expansor(char **line, char **envp);
int	search_var(char **envp, const char* var);
e_errors change_var(char *key, char *newvalue, t_environ *environ);
e_errors add_var(char *key, char *value, t_environ *environ);
char *getkey(char *var);
void unquote(char *str);
void	free_arr(char **s);
void	handle_sigint_vis(int signal);
int is_doublequoted(char *str, int original_flag);
int countargs(t_task *task);

// ...



#endif
