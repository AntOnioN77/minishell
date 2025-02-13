#include "../libft/headers/libft.h"
#include "../minishell.h"
//compilar cc skpiqprueba.c ../libft/libft.a -L../libft -lft

int skip_quotes(char **strpnt, char *end)
{
	char *tmp;

		tmp = *strpnt;
		while (**strpnt == '"' || **strpnt == 39)
		{
			if(**strpnt == '"' && ft_strnchr((*strpnt) +1, '"', (end - *strpnt))) //sin  +1 strchr encontraría el propio caracter de partida
				*strpnt = 1 + ft_strnchr(*strpnt +1, '"', (end - *strpnt));
			else if(**strpnt == 39 && ft_strnchr(*strpnt +1, 39, (end - *strpnt))) //39 es ' en ascii
				*strpnt = 1 + ft_strnchr(*strpnt +1, 39, (end - *strpnt));
			else
				break ;
		}
		if (*strpnt != tmp)
			return (1);
		return (0);
}

#include <stdio.h>
int main(void)
{
	
	char *str= "|'";
	//printf("%s\n",str+33);
	str= str +1;
	skip_quotes(&str, str +1);
	printf("%s\n",str);
	//printf("%s\n", DELIMITERS);
	
	return(0);
}