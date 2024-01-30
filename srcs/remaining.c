#include "ft_ls.h"

t_list	*get_remaining_lst(t_args args, t_stat_list *stats)
{
	t_list	*lst = NULL;
	
	for (int i = 0, j = 0; i < args.argc; j++)
	{
		if (args.argv[j])
		{
			ft_lstadd_back(&lst, ft_lstnew(init_content(stats->stat_buf, args.argv[j])));
			stats = stats->next;
			i++;
		}
	}
	return (lst);
}

int	treat_remaining(t_args args, t_stat_list *stats)
{
	t_list	*lst;
	t_list	*tmp;
	int		ret;

	lst = get_remaining_lst(args, stats);
	sort_list(lst, args.flags);
	tmp = lst;
	ret = 0;
	while (tmp)
	{
		ret |= list_folder_content(((t_content *) tmp->content)->name, args.flags, args.show_title);
		tmp = tmp->next;
		if (tmp && args.show_title)
			ft_putchar_fd('\n', 1);
	}
	ft_lstclear(&lst, free_content);
	return (ret);
}