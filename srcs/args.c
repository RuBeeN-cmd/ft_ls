#include "ft_ls.h"

t_args	parse_args(int argc, char *argv[])
{
	t_args	args;

	args.flags = parse_flags(&argc, argv);
	args.show_title = argc > 1;
	args.argc = argc;
	args.argv = argv;
	args.stats = NULL;
	return (args);
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

int	get_args_stats(t_args *args, t_stat_list **stats)
{
	struct stat	stat_buf;
	int			new_argc;
	int			ret;

	ret = 0;
	*stats = NULL;
	new_argc = args->argc;
	for (int i = 0, j = 0; i < args->argc; j++)
	{
		if (args->argv[j])
		{
			if ((args->flags & LONG && lstat(args->argv[j], &stat_buf) != -1)
				|| stat(args->argv[j], &stat_buf) != -1)
				stat_lstadd_back(stats, stat_buf);
			else
			{
				char	quote = '\'';
				if (ft_strchr(args->argv[j], '\''))
					quote = '\"';
				ft_printf("ls: cannot access %c%s%c: %s\n", quote, args->argv[j], quote, strerror(errno));
				ret = 2;
				new_argc--;
				args->argv[j] = NULL;
			}
			i++;
		}
	}
	args->argc = new_argc;
	return (ret);
}