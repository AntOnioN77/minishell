NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -I/libft/headers
DBGFLAGS = -g3
LDFLAGS = -L ./libft -lft -lreadline
#Cosas del MAC
CFLAGS += -I/usr/local/opt/readline/include
LDFLAGS += -L/usr/local/opt/readline/lib -lreadline

SOURCES = main.c parser/4_check_tree.c\
parser/2_build_tree/constructors.c parser/1_expansor/expansor_utils.c\
parser/1_expansor/expansor.c parser/2_build_tree/get_redir.c\
parser/2_build_tree/build_tree.c parser/2_build_tree/str_utils.c\
executor/executor.c free/free.c free/free_tree.c executor/process.c signal.c\
builtins/non_pipable_builtin.c builtins/ft_cd.c builtins/ft_exit.c\
builtins/ft_export.c builtins/ft_unset.c builtins/builtins.c\
executor/apply_redirs.c executor/path.c cmd_tree_builder.c\
parser/create_heredoc.c parser/3_touch_up_tree.c handler_error.c\
executor/child.c environment/create_envp.c environment/get_envp.c\
environment/init_envp.c environment/var_operation.c\
parser/1_expansor/expand_one.c parser/2_build_tree/unquote.c\
parser/2_build_tree/files.c

OBJECTS = $(SOURCES:.c=.o)
HEADERS = libft/headers/libft.h minishell.h executor.h enums.h structs.h

all: $(NAME)
debug: CFLAGS += $(DBGFLAGS)
debug: fclean libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
$(NAME): libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)

libft/libft.a:
	make extra -C libft
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
clean:
	rm -f $(OBJECTS)
	make clean -C libft
fclean: clean
	rm -f $(NAME)
	rm -f libft/libft.a
re: fclean all
.PHONY: clean fclean all
