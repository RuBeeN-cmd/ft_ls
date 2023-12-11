#include "ft_ls.h"

int	get_nb_elem(char *elem[])
{
	int	len = 0;
	while (elem[len])
		len++;
	return (len);
}

int	get_max_len_n(char *tab[], int n)
{
	int	i = 0;
	int	max = 0;
	n--;
	while (tab[i] && n--)
	{
		int len = ft_strlen(tab[i]);
		if (len > max)
			max = len;
		i++;
	}
	return (len);
}
#include <sys/ioctl.h>
void	print_column(char *elem[])
{
	int nb_elem = get_nb_elem(elem);
	int	nb_lines = 1;
	while (nb_lines)
	{
		int	line_width = 0;
		for (size_t i = 0; i < (nb_elem + 1) / nb_lines; i++)
		{
			line_width += get_max_len_n(&elem[nb_lines * i], nb_lines);
		}
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		if (line_width < w.ws_col)
		{
			printf("nb lines: %d\n");
			nb_lines = -1;
		}
		nb_lines++;
	}
}