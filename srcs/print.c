#include "ft_ls.h"

char	get_file_type(mode_t mode)
{
	// Remaining : 'C', 'D', 'M', 'n', 'P', 's', 'S', 'm'
	if (S_ISREG(mode))
		return ('-');
	else if (S_ISDIR(mode))
		return ('d');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISFIFO(mode))
		return ('p');
	else if (S_ISLNK(mode))
		return ('l');
	return ('?');
}

// char	*get_file_mod(mode_t mode)
// {
// 	char	perm[9];
// 	for (size_t i = 0; i < sizeof(perm); i++)
// 		perm[i] = '-';
// 	for (size_t i = 0; i < sizeof(perm); i++)
// 	{
// 		if (mode & (1 << (sizeof(perm) - i - 1)))
// 		{
// 			if (i % 3 == 0)
// 				perm[i] = 'r';
// 			else if (i % 3 == 1)
// 				perm[i] = 'w';
// 			else
// 				perm[i] = 'x';
// 		}
// 	}
// 	return (perm);
// }

// char	*get_file_nb_link(nlink_t nb_links)
// {
// 	return (ft_itoa(nb_links));
// }

// char	*get_file_owner(uid_t owner_id)
// {
// 	return (getpwuid(owner_id)->pw_name);
// }

// char	*get_file_group(gid_t group_id)
// {
// 	return (getgrgid(group_id)->gr_name);
// }

// char	*get_file_size(off_t size)
// {
// 	return (getgrgid(group_id)->gr_name);
// }

void	show_long_file(char name[])
{
	struct stat statbuf;
	stat(name, &statbuf);
	ft_putchar_fd(get_file_type(statbuf.st_mode), 1);
	ft_putchar_fd(' ', 1);
	ft_putendl_fd(name, 1);
}

void	show_list(t_list *lst, int flags)
{
	if (flags & LONG)
	{
		while (lst)
		{
			show_long_file(((struct dirent *) lst->content)->d_name);
			lst = lst->next;
		}
	}
	while (lst)
	{
		printf("%s\n", ((struct dirent *) lst->content)->d_name);
		lst = lst->next;
	}
}