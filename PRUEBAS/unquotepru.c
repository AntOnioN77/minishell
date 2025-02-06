#include "../libft/headers/libft.h"
#include "../minishell.h"
//compilar cc unquotepru.c ../libft/libft.a -L../libft -lft

void unquote(char *str)
{
	char *newstr;
	char *pntnew;
	int quotlen;
	char *pntstr;

	pntstr = str;
    if (!str || !*str)
		return;
	newstr = ft_calloc(ft_strlen(pntstr) + 1, sizeof(char));
	pntnew = newstr;
	while(*pntstr)
	{
		if ((*pntstr == 39 || *pntstr == '"') &&  ft_strchr(pntstr +1, *pntstr))
		{
			quotlen = ft_strchr(pntstr +1, *pntstr) - (pntstr +1) ;
			ft_strlcpy(pntnew, pntstr +1, quotlen +1);
			pntnew = pntnew + quotlen;
			pntstr = ft_strchr(pntstr +1, *pntstr);
		}
		else
		{
			ft_strlcpy(pntnew, pntstr, 2);
			pntnew++;
		}
		pntstr++;
	}
	ft_strlcpy(str, newstr, 1 + ft_strlen(str));
	free(newstr);
}

#include <stdio.h>
int main(void)
{
	
	char *str= strdup("|''");
/*
	str= strdup("A'BB'C");
	unquote(str);
	printf("%s\n",str);
	
	str= strdup("'ABBC'");
	unquote(str);
	printf("%s\n",str);
	
	str= strdup("ABBC''");
	unquote(str);
	printf("%s\n",str);
	
	str= strdup("''ABBC");
	unquote(str);
	printf("%s\n",str);

	str= strdup("'ABBC");
	unquote(str);
	printf("%s\n",str);
*/
	str= strdup("ABBC'");
	unquote(str);
	printf("%s\n",str);

	return(0);
}