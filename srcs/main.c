#include "ft_ls.h"

void	parse_args(t_args *args, int argc, char *argv[])
{
	args->flags = 0;
	args->argc = 0;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1])
			args->flags |= parse_flags(argv[i]);
		else
			args->argc++;
	}
	args->argv = argv;
}

t_list	*fill_list(DIR *dir, int flags)
{
	t_list	*lst;

	struct dirent	*dirent;
	if (!(dirent = readdir(dir)))
		return (NULL);
	if (dirent->d_name[0] == '.' && !(flags & ALL))
		return (fill_list(dir, flags));
	lst = ft_lstnew(dirent);
	lst->next = fill_list(dir, flags);
	return (lst);
}

void	swap(t_list *l1, t_list *l2)
{
	void	*tmp = l1->content;
	l1->content = l2->content;
	l2->content = tmp;
}

int	is_less(t_list *l1, t_list *l2, int flags)
{
	if (!l1 || !l2)
		return (1);
	(void) flags;
	char	*n1 = ((struct dirent *) l1->content)->d_name;
	char	*n2 = ((struct dirent *) l2->content)->d_name;
	int i = 0;
	int j = 0;
	while (n1[i] == '.' || n1[i] == '_')
		i++;
	while (n2[j] == '.' || n2[j] == '_')
		j++;
	while (n1[i] && ft_tolower(n1[i]) == ft_tolower(n2[j]))
	{
		i++;
		j++;
		while (n1[i] == '.' || n1[i] == '_')
			i++;
		while (n2[j] == '.' || n2[j] == '_')
			j++;
	}
	if (n1[i] || n2[j])
		return (ft_tolower(n1[i]) < ft_tolower(n2[j]));
	i = 0;
	j = 0;
	while (n1[i] == '.' || n1[i] == '_')
		i++;
	while (n2[j] == '.' || n2[j] == '_')
		j++;
	while (n1[i] && n1[i] == n2[j])
	{
		i++;
		j++;
		while (n1[i] == '.' || n1[i] == '_')
			i++;
		while (n2[j] == '.' || n2[j] == '_')
			j++;
	}
	if (n1[i] || n2[j])
		return (n1[i] > n2[j]);
	i = 0;
	while (n1[i] && n1[i] == n2[i])
		i++;
	return (n1[i] < n2[i]);
}

t_list	*get_min(t_list *lst, int flags)
{
	if (!lst)
		return (NULL);
	t_list	*next_min = get_min(lst->next, flags);
	if (is_less(lst, next_min, flags))
		return (lst);
	return (next_min);
}

void	sort_list(t_list *lst, int flags)
{
	if (!lst)
		return ;
	t_list	*min = get_min(lst, flags);
	swap(lst, min);
	sort_list(lst->next, flags);
}

void	show_list(t_list *lst, int flags)
{
	(void) flags;

	while (lst)
	{
		printf("%s\n", ((struct dirent *) lst->content)->d_name);
		lst = lst->next;
	}
}

void	list_folder_content(char folder_name[], int flags)
{
	t_list	*lst;
	DIR		*dir;
	if ((dir = opendir(folder_name)))
	{
		lst = fill_list(dir, flags);
		sort_list(lst, flags);
		show_list(lst, flags);
		if (closedir(dir) == -1)
			printf("Closedir error\n");
	}
	else
		printf("Opendir error\n");
}

int main(int argc, char *argv[])
{
	t_args	args;
	parse_args(&args, argc - 1, argv + 1);
	if (!args.argc)
		list_folder_content(".", args.flags);
	else
	{
		for (int i = 0; args.argc; i++)
		{
			if (args.argv[i][0])
			{
				list_folder_content(args.argv[i], args.flags);
				args.argc--;
			}
		}
	}
	return (0);
}
