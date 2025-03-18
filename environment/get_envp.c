#include "../minishell.h"
#include "../libft/headers/libft.h"
#include <errno.h>
#include <linux/limits.h>

//NO alloca memoria, debuelve un puntero a la cadena original en envp, a continuacion de '='
char	*ft_getenv(const char *name, char *envp[])
{
	int		pos;
	char	*word_start;

	pos = search_var(envp, name);
	if (pos == -1)
		return (NULL);
	word_start = ft_strchr(envp[pos], '=') + 1;
	return (word_start);
}
