#include "ft_ls.h"

char	*get_file_path(char *folder_path, char *name)
{
	char	*path;
	int		folder_path_len;
	int		name_len;
	int		path_len;

	folder_path_len = ft_strlen(folder_path);
	name_len = ft_strlen(name);
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
	ft_strlcat(path, name, path_len);
	return (path);
}

int	is_dot_folder(char name[])
{
	return (!ft_strncmp(name, ".", 2) || !ft_strncmp(name, "..", 3));
}

t_list	*create_content_lst(int flags, struct dirent *dirent, char folder_path[])
{
	(void) flags;
	t_list	*lst = ft_lstnew(malloc(sizeof(t_content)));
	((t_content *) lst->content)->dirent = dirent;
	((t_content *) lst->content)->path = get_file_path(folder_path, dirent->d_name);
	((t_content *) lst->content)->name = ft_strdup(dirent->d_name);
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
	lst->next = fill_list(dir, flags, folder_path);
	return (lst);
}

void	call_list_folder_foreach_folder(t_list *lst, int flags)
{
	while (lst)
	{
		if (flags & RECURSIVE && ((t_content *) lst->content)->stat_buf.st_mode & S_IFDIR
			&& !is_dot_folder(((t_content *) lst->content)->name))
		{
			ft_putchar_fd('\n', 1);
			list_folder_content(((t_content *) lst->content)->path, flags, 1);
		}
		lst = lst->next;
	}
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
		ft_printf("ls: cannot open directory '%s': Permission denied\n", folder_name);
}