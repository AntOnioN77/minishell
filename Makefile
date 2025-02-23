NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -I/libft/headers 
DBGFLAGS = -g3
LDFLAGS = -L ./libft -lft -lreadline
SOURCES = main.c mooks.c parser/check_tree.c parser/constructors.c\
parser/expansor_utils.c parser/expansor.c parser/free_tree.c parser/get_redir.c\
parser/processline.c parser/str_utils.c executor/executor.c\
executor/libexe.c executor/process.c non_pipable_builtin.c signal.c history.c\
executor/apply_redirs.c GNL/get_next_line.c\
parser/tmp_file_name.c create_envp.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = libft/headers/libft.h minishell.h executor.h
SRC_PARS_TEST = mooks.c parser/constructors.c\
parser/expansor_utils.c parser/expansor.c parser/free_tree.c parser/get_redir.c\
parser/processline.c parser/str_utils.c executor/executor.c\
executor/libexe.c executor/process.c non_pipable_builtin.c signal.c history.c\
executor/apply_redirs.c GNL/get_next_line_utils.c GNL/get_next_line.c\
parser/tmp_file_name.c
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
