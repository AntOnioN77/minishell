#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE

#  define BUFFER_SIZE 15

# endif

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "../libft/headers/libft.h"

//size_t	gnl_strlen(const char *s);
int		gnl_strchr(const char *s, int c);
//char	*gnl_substr(char const *s, unsigned int start, size_t len);
char	*gnl_strjoin(char *s, char const *s2);
//char	*gnl_strdup(const char *s);
//int		put_line(char **line, char **aux, int reader);
int		get_next_line(int fd, char **line);

#endif