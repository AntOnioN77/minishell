NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -I/libft/headers 
DBGFLAGS = -g3
LDFLAGS = -L ./libft -lft -lreadline
SOURCES = main.c mooks.c parser/check_tree.c parser/constructors.c\
parser/expansor_utils.c parser/expansor.c parser/free_tree.c parser/get_redir.c\
parser/processline.c parser/str_utils.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = libft/headers/libft.h minishell.h

all: $(NAME)
debug: CFLAGS += $(DBGFLAGS)
debug: fclean libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -I/trace_tools $(OBJECTS) $(LDFLAGS) -o $(NAME)
$(NAME): libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
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
