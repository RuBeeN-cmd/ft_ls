#include "ft_ls.h"

typedef struct	s_long_line
{
	char	type;
	char	perm[10];
	char	*nb_links;
	char	*owner_usr;
	char	*owner_group;
	char	*size;
	char	date[13];
	char	*name;
}				t_long_line;

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

void	fill_perm(char *perm_buff, mode_t mode)
{
	perm_buff[9] = 0;
	for (size_t i = 0; i < 9; i++)
	{
		if (mode & (1 << (9 - i - 1)))
		{
			if (i % 3 == 0)
				perm_buff[i] = 'r';
			else if (i % 3 == 1)
				perm_buff[i] = 'w';
			else
				perm_buff[i] = 'x';
		}
		else
			perm_buff[i] = '-';
	}
}

void	fill_date(char *date_buff, time_t ts)
{
	char *new_buf = ctime(&ts);
	ft_strlcpy(date_buff, &(new_buf[4]), 13);
}

void	fill_long_line(t_long_line *long_line, char *name)
{
	struct stat statbuf;
	stat(name, &statbuf);
	long_line->type = get_file_type(statbuf.st_mode);
	fill_perm(long_line->perm, statbuf.st_mode);
	long_line->nb_links = ft_itoa(statbuf.st_nlink);
	long_line->owner_usr = ft_strdup(getpwuid(statbuf.st_uid)->pw_name);
	long_line->owner_group = ft_strdup(getgrgid(statbuf.st_gid)->gr_name);
	long_line->size = ft_itoa(statbuf.st_size);
	fill_date(long_line->date, statbuf.st_mtime);
	long_line->name = name;
}

void	print_long_line(t_long_line long_line, unsigned int *len)
{
	ft_printf("%c%s ", long_line.type, long_line.perm);
	int space_nb = len[0] - ft_strlen(long_line.nb_links);
	while (space_nb-- > 0)
		ft_putchar_fd(' ', 1);
	ft_printf("%s ", long_line.nb_links);

	ft_printf("%s ", long_line.owner_usr);
	space_nb = len[1] - ft_strlen(long_line.owner_usr);
	while (space_nb-- > 0)
		ft_putchar_fd(' ', 1);

	ft_printf("%s ", long_line.owner_group);
	space_nb = len[2] - ft_strlen(long_line.owner_group);
	while (space_nb-- > 0)
		ft_putchar_fd(' ', 1);

	space_nb = len[3] - ft_strlen(long_line.size);
	while (space_nb-- > 0)
		ft_putchar_fd(' ', 1);
	ft_printf("%s %s %s\n",
		long_line.size, long_line.date, long_line.name);
}

void	show_list(t_list *lst, int flags)
{
	if (flags & LONG)
	{
		t_long_line	*lines = malloc(sizeof(t_long_line) * ft_lstsize(lst));
		int i = 0;
		t_list	*tmp = lst;
		unsigned int	max_len[4];
		ft_bzero(max_len, sizeof(max_len));
		while (tmp)
		{
			fill_long_line(&(lines[i]), ((struct dirent *) tmp->content)->d_name);
			if (ft_strlen(lines[i].nb_links) > max_len[0])
				max_len[0] = ft_strlen(lines[i].nb_links);
			if (ft_strlen(lines[i].owner_usr) > max_len[1])
				max_len[1] = ft_strlen(lines[i].owner_usr);
			if (ft_strlen(lines[i].owner_group) > max_len[2])
				max_len[2] = ft_strlen(lines[i].owner_group);
			if (ft_strlen(lines[i].size) > max_len[3])
				max_len[3] = ft_strlen(lines[i].size);
			tmp = tmp->next;
			i++;
		}
		i = 0;
		while (lst)
		{

			print_long_line(lines[i], max_len);
			free(lines[i].owner_usr);
			free(lines[i].owner_group);
			i++;
			lst = lst->next;
		}
		free(lines);
		
	}
	while (lst)
	{
		ft_printf("%s\n", ((struct dirent *) lst->content)->d_name);
		lst = lst->next;
	}
}