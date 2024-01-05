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

void	fill_date(char *date_buff, time_t ts)
{
	char *new_buf = ctime(&ts);
	ft_strlcpy(date_buff, &(new_buf[4]), 13);
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
	line->size = ft_itoa(content->stat_buf.st_size);
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
	print_spaces(len[3] - ft_strlen(line.size));
	ft_printf("%s %s %s\n", line.size, line.date, line.name);
}

void	show_long_format(t_list *lst)
{
	int len = ft_lstsize(lst);
	t_line	*lines = malloc(sizeof(t_line) * len);
	int i = 0;
	t_list	*tmp = lst;
	unsigned int	max_len[4];
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
		if (ft_strlen(lines[i].size) > max_len[3])
			max_len[3] = ft_strlen(lines[i].size);
		tmp = tmp->next;
		i++;
	}
	i = 0;
	ft_printf("total %u\n", total_size);
	while (i < len)
	{
		print_line(lines[i], max_len);
		free_line(lines[i]);
		i++;
	}
	free(lines);
}