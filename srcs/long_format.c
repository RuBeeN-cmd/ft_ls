#include "ft_ls.h"

void	free_line(t_line line)
{
	free(line.size);
	free(line.nb_links);
	free(line.owner_usr);
	free(line.owner_group);
}

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

int	get_nbr_len(long long n)
{
	int	i;

	i = 0;
	if (!n)
		return (1);
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_ultoa(unsigned long n)
{
	char	*str;
	int		i;

	str = malloc(sizeof(char) * (get_nbr_len(n) + 1));
	if (!str)
		return (NULL);
	i = get_nbr_len(n);
	if (!n)
		str[0] = '0';
	str[i] = 0;
	while (n)
	{
		str[--i] = n % 10 + '0';
		n /= 10;
	}
	return (str);
}

void	fill_date(char *date_buff, time_t ts)
{
	char *new_buf = ctime(&ts);
	ft_strlcpy(date_buff, &(new_buf[4]), 13);
}
void	fill_size(t_line *line, struct stat stat_buf)
{
	line->size = NULL;
	line->major = NULL;
	line->minor = NULL;
	if (stat_buf.st_mode & S_ISCHR)
	{
		line->major = ft_ultoa(major(stat_buf.st_dev));
		line->minor = ft_ultoa(minor(stat_buf.st_dev));
	}
	else
		line->size = ft_ultoa(stat_buf.st_size);
}
void	fill_line(t_line *line, t_content *content)
{
	line->type = get_file_type(content->stat_buf.st_mode);
	fill_perm(line->perm, content->stat_buf.st_mode);
	line->nb_links = ft_itoa(content->stat_buf.st_nlink);
	struct passwd	*pwuid;
	pwuid = getpwuid(content->stat_buf.st_uid);
	if (!pwuid)
		line->owner_usr = ft_itoa(content->stat_buf.st_uid);
	else
		line->owner_usr = ft_strdup(pwuid->pw_name);
	struct group	*grgid;
	grgid = getgrgid(content->stat_buf.st_gid);
	if (!grgid)
		line->owner_group = ft_itoa(content->stat_buf.st_gid);
	else
		line->owner_group = ft_strdup(grgid->gr_name);
	fill_size(&line->size, content->stat_buf);
	fill_date(line->date, content->stat_buf.st_mtime);
	line->name = content->name;
}

void	print_spaces(int n)
{
	while (n--)
		ft_putchar_fd(' ', 1);
}

void	print_line(t_line line, unsigned int *len)
{
	if (!len)
		return ;
	ft_printf("%c%s ", line.type, line.perm);
	print_spaces(len[0] - ft_strlen(line.nb_links));
	ft_printf("%s ", line.nb_links);
	ft_printf("%s ", line.owner_usr);
	print_spaces(len[1] - ft_strlen(line.owner_usr));
	ft_printf("%s ", line.owner_group);
	print_spaces(len[2] - ft_strlen(line.owner_group));
	if (line.size)
	{
		print_spaces(len[3] - ft_strlen(line.size));
		ft_printf("%s ", line.size);
	}
	else
	{
		print_spaces(len[4] - ft_strlen(line.major));
		ft_printf("%s, ", line.major);
		print_spaces(len[5] - ft_strlen(line.minor));
		ft_printf("%s ", line.minor);
	}
	ft_printf("%s %s\n", line.date, line.name);
}

void	show_long_format(t_list *lst, int is_reg)
{
	int len = ft_lstsize(lst);
	t_line	*lines = malloc(sizeof(t_line) * len);
	int i = 0;
	t_list	*tmp = lst;
	unsigned int	max_len[6];
	ft_bzero(max_len, sizeof(max_len));
	unsigned int	total_size = 0;
	while (tmp)
	{
		total_size += 512 * ((t_content *) tmp->content)->stat_buf.st_blocks / 1024; 
		fill_line(&(lines[i]), (t_content *) tmp->content);
		if (ft_strlen(lines[i].nb_links) > max_len[0])
			max_len[0] = ft_strlen(lines[i].nb_links);
		if (ft_strlen(lines[i].owner_usr) > max_len[1])
			max_len[1] = ft_strlen(lines[i].owner_usr);
		if (ft_strlen(lines[i].owner_group) > max_len[2])
			max_len[2] = ft_strlen(lines[i].owner_group);
		if (lines[i].size)
		{
			if (ft_strlen(lines[i].size) > max_len[3])
				max_len[3] = ft_strlen(lines[i].size);
		}
		else if (ft_strlen(lines[i].major) + ft_strlen(lines[i].minor) + 2 > max_len[3])
			max_len[3] = ft_strlen(lines[i].size);
		if (lines[i].major)
			max_len[3] = ft_strlen(lines[i].size);

		tmp = tmp->next;
		i++;
	}
	i = 0;
	if (!is_reg)
		ft_printf("total %u\n", total_size);
	while (i < len)
	{
		print_line(lines[i], max_len);
		free_line(lines[i]);
		i++;
	}
	free(lines);
}