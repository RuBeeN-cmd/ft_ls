#include "ft_ls.h"

t_args	init_args(int argc, char *argv[])
{
	t_args	args;

	args.flags = 0;
	args.show_title = 0;
	args.argc = argc;
	args.argv = argv;
	args.stats = NULL;
	return (args);
}

void	parse_args(t_args *args, int argc, char *argv[])
{
	*args = init_args(argc, argv);
	parse_flags(args, argc, argv);
	if (args->argc > 1)
		args->show_title = 1;
}

void	stat_lstadd_back(t_stat_list **lst, struct stat value)
{
	t_stat_list	*tmp;

	tmp = *lst;
	if (lst)
	{
		if (!*lst)
		{
			*lst = malloc(sizeof(t_stat_list));
			(*lst)->next = NULL;
			(*lst)->stat_buf = value;
		}
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = malloc(sizeof(t_stat_list));
			tmp->next->next = NULL;
			tmp->next->stat_buf = value;
		}
	}
}

void	del_stat_lst(t_stat_list *lst)
{
	if (!lst)
		return ;
	del_stat_lst(lst->next);
	free(lst);
}

t_stat_list	*get_args_stats(t_args *args)
{
	t_stat_list	*stats;
	struct stat	stat_buf;
	int			new_argc;

	stats = NULL;
	new_argc = args->argc;
	for (int i = 0, j = 0; i < args->argc; j++)
	{
		if (args->argv[j])
		{
			if ((args->flags & LONG && lstat(args->argv[j], &stat_buf) != -1)
				|| stat(args->argv[j], &stat_buf) != -1)
				stat_lstadd_back(&stats, stat_buf);
			else
			{
				ft_printf("ls: cannot access '%s': No such file or directory\n", args->argv[j]);
				new_argc--;
				args->argv[j] = NULL;
			}
			i++;
		}
	}
	args->argc = new_argc;
	return (stats);
}