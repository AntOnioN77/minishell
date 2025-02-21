NAME = libft.a
CC = clang
FLAGS = -Wall -Wextra -Werror
HEADERS = headers/libft.h
SOURCES = src/ft_atoi.c src/ft_bzero.c src/ft_calloc.c\
src/ft_isalnum.c src/ft_isalpha.c src/ft_isascii.c src/ft_isdigit.c\
src/ft_isprint.c src/ft_memchr.c src/ft_memcmp.c src/ft_memcpy.c\
src/ft_memmove.c src/ft_memset.c src/ft_strchr.c src/ft_strrchr.c\
src/ft_strlcat.c src/ft_strlcpy.c src/ft_strlen.c src/ft_strncmp.c\
src/ft_strnstr.c src/ft_strdup.c src/ft_tolower.c src/ft_toupper.c\
src/ft_substr.c src/ft_strjoin.c src/ft_split.c src/ft_itoa.c\
src/ft_putnbr_fd.c src/ft_putendl_fd.c src/ft_putchar_fd.c\
src/ft_putstr_fd.c src/ft_strmapi.c src/ft_striteri.c src/ft_strtrim.c\
src/ft_strcmp.c src/ft_atol.c src/ft_atod.c src/ft_putnbr_base.c\
src/ft_putunbr_base.c src/ft_strnchr.c
BONUSSRC = src/ft_lstnew.c src/ft_lstadd_front.c src/ft_lstsize.c\
src/ft_lstlast.c src/ft_lstadd_back.c src/ft_lstdelone.c src/ft_lstclear.c\
src/ft_lstiter.c src/ft_lstmap.c 
EXTRASRC =ring_array/ring_next_index.c ring_array/ring_prev_index.c\
ring_array/ring_free.c ring_array/ring_init.c\
ring_array/ring_get_head.c ring_array/ring_get_tail.c\
ring_array/ring_pop_head.c ring_array/ring_add_head.c\
ring_array/ring_rotate.c ring_array/ring_reverse_rotate.c\
ring_array/ring_swap.c ring_array/ring_push.c ring_array/ring_find_bigest.c\
ring_array/ring_find_smallest.c ring_array/ring_find_value.c\
ring_array/ring_get_level.c ring_array/ring_index_get_level.c\
ring_array/ring_has_duplicates.c\
parsing/ft_has_duplicates.c parsing/ft_is_in_bounds_int.c

OBJECTS = $(SOURCES:.c=.o)
BONUSOBJ = $(BONUSSRC:.c=.o)
EXTRAOBJ = $(EXTRASRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS) $(HEADERS)
	ar rcs $(NAME) $(OBJECTS)


extra: HEADERS += headers/ring.h headers/parsing.h
extra: $(OBJECTS) $(BONUSOBJ) $(EXTRAOBJ)
	ar rcs $(NAME) $(OBJECTS) $(BONUSOBJ) $(EXTRAOBJ)

bonus: $(OBJECTS) $(BONUSOBJ)
	ar rcs $(NAME) $(OBJECTS) $(BONUSOBJ)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(BONUSOBJ) $(EXTRAOBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus extra