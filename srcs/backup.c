

// int	parse_no_files(int argc, char *argv[])
// {
// 	int	status = 0;
// 	for (int i = 0; i < argc; i++)
// 	{
// 		if (argv[i][0])
// 		{
// 			struct stat	stat_;
// 			if (stat(argv[i], &stat_) == -1)
// 			{
// 				printf("ft_ls: cannot access '%s': %s\n", argv[i], strerror(errno));
// 				status = 2;
// 				argv[i][0] = 0;
// 			}
// 		}
// 	}
// 	return (status);
// }

// void	parse_files(int argc, char *argv[], int flags)
// {
// 	(void) flags;
// 	for (int i = 0; i < argc; i++)
// 	{
// 		if (argv[i][0])
// 		{
// 			struct stat	stat_;
// 			stat(argv[i], &stat_);
// 			if (!S_ISDIR(stat_.st_mode))
// 			{
// 				printf("file : %s\n", argv[i]);
// 				argv[i][0] = 0;
// 			}
// 			else if (flags & LONG && lstat(argv[i], &stat_) != -1)
// 			{
// 				printf("symlink : %s\n", argv[i]);
// 				argv[i][0] = 0;
// 			}
// 		}
// 	}
// }

// t_list	*fill_list(DIR *dir, int flags)
// {
// 	t_list	*lst;

// 	struct dirent	*dirent;
// 	if (!(dirent = readdir(dir)))
// 		return (NULL);
// 	if (dirent->d_name[0] == '.' && !(flags & ALL))
// 		return (fill_list(dir, flags));
// 	lst = ft_lstnew(dirent);
// 	lst->next = fill_list(dir, flags);
// 	return (lst);
// }

// void	swap(t_list *l1, t_list *l2)
// {
// 	void	*tmp = l1->content;
// 	l1->content = l2->content;
// 	l2->content = tmp;
// }

// int	is_less(t_list *l1, t_list *l2, int flags)
// {
// 	if (!l1 || !l2)
// 		return (1);
// 	(void) flags;
// 	char	*n1 = ((struct dirent *) l1->content)->d_name;
// 	char	*n2 = ((struct dirent *) l2->content)->d_name;
// 	int i = 0;
// 	while (n1[i] && n1[i] == n2[i])
// 		i++;
// 	return (n1[i] < n2[i]);
// }

// t_list	*get_min(t_list *lst, int flags)
// {
// 	if (!lst)
// 		return (NULL);
// 	t_list	*next_min = get_min(lst->next, flags);
// 	if (is_less(lst, next_min, flags))
// 		return (lst);
// 	return (next_min);
// }

// void	sort_list(t_list *lst, int flags)
// {
// 	if (!lst)
// 		return ;
// 	t_list	*min = get_min(lst, flags);
// 	swap(lst, min);
// 	sort_list(lst->next, flags);
// }

// void	show_list(t_list *lst, int flags)
// {
// 	(void) flags;

// 	while (lst)
// 	{
// 		printf("%s\n", ((struct dirent *) lst->content)->d_name);
// 		lst = lst->next;
// 	}
// }

// void	list_folder_content(char folder_name[], int flags)
// {
// 	t_list	*lst;
// 	DIR		*dir;
// 	if ((dir = opendir(folder_name)))
// 	{
// 		lst = fill_list(dir, flags);
// 		sort_list(lst, flags);
// 		show_list(lst, flags);
// 		if (closedir(dir) == -1)
// 			printf("Closedir error\n");
// 	}
// 	else
// 		printf("Opendir error\n");
// }

// void	parse_folder(int argc, char *argv[], int flags)
// {
// 	(void) flags;
// 	for (int i = 0; i < argc; i++)
// 	{
// 		if (argv[i][0])
// 		{
// 			list_folder_content(argv[i], flags);
// 			argv[i][0] = 0;
// 		}
// 	}
// }
