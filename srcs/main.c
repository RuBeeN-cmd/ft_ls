#include "ft_ls.h"

char	*get_file_path(char *folder_path, struct dirent *dirent)
{
	char	*path;
	int		folder_path_len;
	int		name_len;
	int		path_len;

	folder_path_len = ft_strlen(folder_path);
	name_len = ft_strlen(dirent->d_name);
	path_len = folder_path_len + name_len + 1;
	if (folder_path[folder_path_len - 1] != '/')
		path_len++;
	path = malloc(sizeof(char) * path_len);
	ft_strlcpy(path, folder_path, folder_path_len + 1);
	if (folder_path[folder_path_len - 1] != '/')
	{
		path[folder_path_len] = '/';
		path[folder_path_len + 1] = 0;
	}
	ft_strlcat(path, dirent->d_name, path_len);
	return (path);
}

int	is_dot_folder(char name[])
{
	return (!ft_strncmp(name, ".", 2) || !ft_strncmp(name, "..", 3));
}

t_list	*create_content_lst(int flags, struct dirent *dirent, char folder_path[])
{
	t_list	*lst = ft_lstnew(malloc(sizeof(t_content)));
	((t_content *) lst->content)->dirent = dirent;
	((t_content *) lst->content)->path = NULL;
	if (flags & (LONG | TIME)
		|| (flags & RECURSIVE && dirent->d_type == DT_DIR
			&& !is_dot_folder(dirent->d_name)))
		((t_content *) lst->content)->path = get_file_path(folder_path, dirent);
	((t_content *) lst->content)->name = dirent->d_name;
	return (lst);
}

t_list	*fill_list(DIR *dir, int flags, char *folder_path)
{
	t_list			*lst;
	struct dirent	*dirent;

	if (!(dirent = readdir(dir)))
		return (NULL);
	if (dirent->d_name[0] == '.' && !(flags & ALL))
		return (fill_list(dir, flags, folder_path));
	lst = create_content_lst(flags, dirent, folder_path);
	if (flags & (LONG | TIME))
		lstat(((t_content *) lst->content)->path, &(((t_content *) lst->content)->stat_buf));
	if ((flags & RECURSIVE && ((t_content *) lst->content)->dirent->d_type != DT_DIR)
		|| (flags & (LONG | TIME) && !(flags & RECURSIVE))
		|| (flags & RECURSIVE && (is_dot_folder(((t_content *) lst->content)->dirent->d_name))))
	{
		free(((t_content *) lst->content)->path);
		((t_content *) lst->content)->path = NULL;
	}
	lst->next = fill_list(dir, flags, folder_path);
	return (lst);
}

void	free_content(void *content)
{
	if (((t_content *) content)->path)
		free(((t_content *) content)->path);
	free(content);
}

void	list_folder_content(char folder_name[], int flags, int multiple_args)
{
	t_list	*lst;
	DIR		*dir;
	if ((dir = opendir(folder_name)))
	{
		lst = fill_list(dir, flags, folder_name);
		sort_list(lst, flags);
		if (multiple_args || flags & RECURSIVE)
			ft_printf("%s:\n", folder_name);
		show_list(lst, flags, 0);
		if (flags & RECURSIVE)
			call_list_folder_foreach_folder(lst, flags);
		ft_lstclear(&lst, free_content);
		if (closedir(dir) == -1)
			ft_printf("Closedir error\n");
	}
	else
		ft_printf("Opendir error\n");
}

void	call_list_folder_foreach_folder(t_list *lst, int flags)
{
	while (lst)
	{
		if (((t_content *) lst->content)->path)
		{
			ft_putchar_fd('\n', 1);
			list_folder_content(((t_content *) lst->content)->path, flags, 1);
			free(((t_content *) lst->content)->path);
			((t_content *) lst->content)->path = NULL;
		}
		lst = lst->next;
	}
}

t_content	*init_content(struct stat stat_buf, char *name)
{
	t_content	*content;

	content = malloc(sizeof(t_content));
	content->stat_buf = stat_buf;
	content->name = name;
	content->path = NULL;
	content->dirent = NULL;
	return (content);
}

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

void	treat_remaining(t_args args, t_stat_list *stats)
{
	t_list	*lst;
	t_list	*tmp;

	lst = get_remaining_lst(args, stats);
	sort_list(lst, args.flags);
	tmp = lst;
	while (tmp)
	{
		list_folder_content(((t_content *) tmp->content)->name, args.flags, args.show_title);
		tmp = tmp->next;
		if (tmp && args.show_title)
			ft_putchar_fd('\n', 1);
	}
	ft_lstclear(&lst, free_content);
}

int main(int argc, char *argv[])
{
	t_args		args;
	t_stat_list	*stats;
	
	parse_args(&args, argc - 1, argv + 1);
	if (!args.argc)
		list_folder_content(".", args.flags, 0);
	else
	{
		stats = get_args_stats(&args);
		treat_reg_file(&args, stats);
		treat_remaining(args, stats);
		del_stat_lst(stats);
	}
	return (0);
}
