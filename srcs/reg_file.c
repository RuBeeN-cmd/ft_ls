#include "ft_ls.h"

t_list	*get_reg_file_lst(t_args *args, t_stat_list *stats)
{
	t_list	*lst = NULL;
	int		new_argc;
	
	new_argc = args->argc;
	for (int i = 0, j = 0; i < args->argc; j++)
	{
		if (args->argv[j])
		{
			if (stats->stat_buf.st_mode & S_IFREG || stats->stat_buf.st_mode & S_IFLNK)
			{
				ft_lstadd_back(&lst, ft_lstnew(init_content(stats->stat_buf, args->argv[j])));
				args->argv[j] = NULL;
				new_argc--;
			}
			stats = stats->next;
			i++;
		}
	}
	args->argc = new_argc;
	return (lst);
}

void	treat_reg_file(t_args *args, t_stat_list *args_stats)
{
	t_list	*lst;

	lst = get_reg_file_lst(args, args_stats);
	if (!lst)
		return ;
	sort_list(lst, args->flags);
	show_list(lst, args->flags, 1);
	if (lst && args->argc)
		ft_putchar_fd('\n', 1);
	ft_lstclear(&lst, free_content);
}