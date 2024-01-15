#include "ft_ls.h"

void	show_list(t_list *lst, int flags, int is_reg)
{
	if (flags & LONG)
		show_long_format(lst, is_reg);
	else
		show_column_format(lst);
}