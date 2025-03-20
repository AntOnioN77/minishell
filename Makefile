NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -I/libft/headers
DBGFLAGS = -g3
LDFLAGS = -L ./libft -lft -lreadline
#Cosas del MAC
CFLAGS += -I/usr/local/opt/readline/include
LDFLAGS += -L/usr/local/opt/readline/lib -lreadline

SOURCES = main.c mooks.c parser/4_check_tree.c parser/2_build_tree/constructors.c\
parser/1_expansor/expansor_utils.c parser/1_expansor/expansor.c parser/2_build_tree/get_redir.c\
parser/2_build_tree/build_tree.c parser/2_build_tree/str_utils.c executor/executor.c\
free/free.c free/free_tree.c executor/process.c signal.c\
builtins/non_pipable_builtin.c builtins/ft_cd.c builtins/ft_exit.c\
executor/apply_redirs.c GNL/get_next_line.c builtins/builtins.c cmd_tree_builder.c\
parser/create_heredoc.c parser/3_touch_up_tree.c handler_error.c\
environment/create_envp.c environment/get_envp.c environment/init_envp.c environment/var_operation.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = libft/headers/libft.h minishell.h executor.h
SRC_PARS_TEST = mooks.c parser/2_build_tree/build_tree/constructors.c\
parser/1_expansor/expansor_utils.c parser/1_expansor/expansor.c parser/2_build_tree/get_redir.c\
free/free.c free/free_tree.c\
parser/2_build_tree/build_tree.c parser/str_utils.c executor/executor.c\
free.c executor/process.c builtins/non_pipable_builtin.c signal.c\
executor/apply_redirs.c GNL/get_next_line_utils.c GNL/get_next_line.c\
parser/create_heredoc.c create_envp.c parser/3_touch_up_tree.c handler_error.c\
environment/create_envp.c environment/get_envp.c environment/init_envp.c environment/var_operation.c
OBJ_PARS_TEST = $(SRC_PARS_TEST:.c=.o)


all: $(NAME)
debug: CFLAGS += $(DBGFLAGS)
debug: fclean libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
$(NAME): libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)

parse_test: debug test/parse-test.o
	$(CC) $(CFLAGS) $(CPPFLAGS) test/parse-test.o $(OBJ_PARS_TEST) $(LDFLAGS) -o parse_test
libft/libft.a:
	make extra -C libft
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
.PHONY: clean fclean all
clean:
	rm -f $(OBJECTS)
	make clean -C libft
fclean: clean
	rm -f $(NAME)
	rm -f libft/libft.a
re: fclean all
