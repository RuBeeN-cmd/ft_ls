#include "ft_ls.h"

void	show_column_format(t_list *lst)
{
	while (lst)
	{
		ft_printf("%s\n", ((t_content *) lst->content)->name);
		lst = lst->next;
	}
}

void	show_list(t_list *lst, int flags)
{
	if (flags & LONG)
		show_long_format(lst);
	else
		show_column_format(lst);
}