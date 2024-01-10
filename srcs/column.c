#include "ft_ls.h"

int	get_win_width()
{
	struct winsize	win_size;

	if (ioctl(1, TIOCGWINSZ, &win_size) == -1)
		return (-1);
	return (win_size.ws_col);
}

t_list	*get_next_file(t_list *lst, int next_iter)
{
	while (lst && next_iter--)
		lst = lst->next;
	return (lst);
}

int	get_max_len(int *max_len, int size, int i)
{
	if (!max_len)
		return (-1);
	if (i >= size)
		return (-1);
	return (max_len[i]);
}

t_list	*get_new_line(t_list *lst, int nb_line)
{
	t_list	*line;
	t_list	*crt_file;

	line = NULL;
	crt_file = lst;
	while (crt_file)
	{
		ft_lstadd_back(&line, ft_lstnew(((t_content *) crt_file->content)->name));
		crt_file = get_next_file(crt_file, nb_line);
	}
	return (line);
}

void	del_line(void *line)
{
	ft_lstclear((t_list **) &line, NULL);
}

void	update_column_width(int *column_width, int width_size, t_list *line)
{
	int	len;
	for (int i = 0; i < width_size && line; i++)
	{
		len = ft_strlen(line->content);
		if (len > column_width[i])
			column_width[i] = len;
		line = line->next;
	}
}

int	get_sum_width(int *tab, int size)
{
	int	ret;

	if (!tab)
		return (-1);
	ret = 0;
	for (int i = 0; i < size; i++)
		ret += tab[i];
	return (ret);	
}

void	print_lines(t_list *print, int *column_width)
{
	int	i;
	while (print)
	{
		t_list	*line = print->content;
		i = 0;
		while (line)
		{
			ft_printf("%s", (char *) line->content);
			if (line->next)
				print_spaces(column_width[i] - ft_strlen(line->content) + 2);
			line = line->next;
			i++;
		}
		print = print->next;
		ft_putchar_fd('\n', 1);
	}
}

t_list	*get_with_n_lines(t_list *lst, int width, int nb_line)
{
	t_list	*print;
	int		*column_width;
	int		column_nb;
	t_list	*line;

	print = NULL;
	column_nb = ft_lstsize(lst) / nb_line + (ft_lstsize(lst) % nb_line != 0);
	column_width = ft_calloc(sizeof(int), column_nb);
	for (int i = 0; i < nb_line && lst; i++)
	{
		line = get_new_line(lst, nb_line);
		lst = lst->next;
		ft_lstadd_back(&print, ft_lstnew(line));
		update_column_width(column_width, column_nb, line);
	}
	if ((get_sum_width(column_width, column_nb) + (column_nb - 1) * 2 > width && column_nb != 1)
	|| (column_nb == 2 && (ft_lstsize(print->next) >= ft_lstsize(print) - 1)))
		ft_lstclear(&print, del_line);
	if (print)
		print_lines(print, column_width);
	free(column_width);
	return (print);
}

void	show_column_format(t_list *lst)
{
	int		width;
	t_list	*print;
	int		crt_line;

	if (!lst)
		return ;
	width = get_win_width();
	crt_line = 1;
	while (!(print = get_with_n_lines(lst, width, crt_line)))
		crt_line++;
	ft_lstclear(&print, del_line);
}