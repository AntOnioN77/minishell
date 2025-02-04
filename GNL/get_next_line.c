#include "get_next_line.h"

int	gnl_strchr(const char *s, int c)
{
	int		i;
	char	*aux;

	i = 0;
	aux = (char *)s;
	while (aux[i] != '\0')
	{
		if (aux[i] == c)
			return (i);
		i++;
	}
	if (aux[i] == c)
		return (i);
	return (-1);
}

char	*gnl_strjoin(char *s1, char const *s2)
{
	char	*dst;
	int		i;
	int		len_s1;
	int		len_s2;

	i = -1;
	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	dst = malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!dst)
		return (NULL);
	while (++i < len_s1)
		dst[i] = s1[i];
	i = -1;
	while (++i < len_s2)
		dst[len_s1 + i] = s2[i];
	dst[len_s1 + i] = '\0';
	free(s1);
	return (dst);
}

int	put_line(char **line, char **aux, int reader)
{
	char	*tmp;
	int		pos;

	if (reader == -1)
		return (-1);
	pos = gnl_strchr(*aux, '\n');
	if (pos >= 0)
	{
		*line = ft_substr(*aux, 0, pos);
		tmp = ft_substr(*aux, pos + 1, ft_strlen(*aux));
		free(*aux);
		*aux = tmp;
		return (1);
	}
	else
	{
		*line = ft_strdup(*aux);
		free(*aux);
		*aux = NULL;
		return (0);
	}
}

int	get_next_line(int fd, char **line)
{
	static char	*aux[4096];
	char		*buff;
	int			reader;
	int			ctrl;

	if (fd < 0 || BUFFER_SIZE <= 0 || !line)
		return (-1);
	buff = malloc(sizeof(char *) * (BUFFER_SIZE + 1));
	if (!buff)
		return (-1);
	if (!aux[fd])
		aux[fd] = ft_strdup("");
	reader = 1;
	while (gnl_strchr(aux[fd], '\n') == -1 && reader > 0)
	{
		reader = read(fd, buff, BUFFER_SIZE);
		if (reader > 0)
		{
			buff[reader] = '\0';
			aux[fd] = gnl_strjoin(aux[fd], buff);
		}
	}
	ctrl = put_line(line, &aux[fd], reader);
	free(buff);
	return (ctrl);
}