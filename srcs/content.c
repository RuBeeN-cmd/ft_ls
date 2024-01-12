#include "ft_ls.h"

t_content	*init_content(struct stat stat_buf, char *name)
{
	t_content	*content;

	content = malloc(sizeof(t_content));
	content->stat_buf = stat_buf;
	content->name = ft_strdup(name);
	content->path = NULL;
	if (stat_buf.st_mode & S_IFLNK)
		content->path = name;
	content->dirent = NULL;
	return (content);
}

void	free_content(void *content)
{
	if (((t_content *) content)->path)
		free(((t_content *) content)->path);
	if (((t_content *) content)->name)
		free(((t_content *) content)->name);
	free(content);
}