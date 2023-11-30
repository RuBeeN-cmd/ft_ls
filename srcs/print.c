#include "ft_ls.h"

typedef struct	s_long_format
{
	char	type;
	char	mod[9];
	char	*nb_links;
	char	*owner;
	char	*group;
	char	*size;
	char	*date;
	char	*name;
}				t_long_format;

char	get_file_type(mode_t mode)
{
	// Remaining : 'C', 'D', 'M', 'n', 'P', 's', 'S', 'm'
	if (mode & S_IFREG)
		return ('-');
	else if (mode & S_IFDIR)
		return ('d');
	else if (mode & S_IFBLK)
		return ('b');
	else if (mode & S_IFCHR)
		return ('c');
	else if (mode & S_IFIFO)
		return ('p');
	else if (mode & S_IFLNK)
		return ('l');
	return ('?');
}

char	*get_file_mod(mode_t mode)
{
	char	perm[9];
	for (int i = 0; i < sizeof(perm); i++)
		perm[i] = '-';
	for (int i = 0; i < sizeof(perm); i++)
	{
		if (mode & (1 << (sizeof(perm) - i - 1)))
		{
			if (i % 3 == 0)
				perm[i] = 'r';
			else if (i % 3 == 1)
				perm[i] = 'w';
			else
				perm[i] = 'x';
		}
	}
	return (perm);
}

char	*get_file_nb_link(nlink_t nb_links)
{
	return (ft_itoa(nb_links));
}

char	*get_file_owner(uid_t owner_id)
{
	return (getpwuid(owner_id)->pw_name);
}

char	*get_file_group(gid_t group_id)
{
	return (getgrgid(group_id)->gr_name);
}

char	*get_file_size(off_t size)
{
	return (getgrgid(group_id)->gr_name);
}

void	show_long_file(char name[])
{
	t_long_format	file;
	struct stat statbuf;
	stat(name, &statbuf);
    printf("%d\n", statbuf.st_mode);
}