
#include <stdlib.h>

int     ft_free_double(char **s)
{
        int     i;

        i = 0;
        if (!s)
                return (0);
        while (s[i])
        {
                free (s[i]);
                i++;
        }
        free(s);
        return (1);
}
