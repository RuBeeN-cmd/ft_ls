#include "ft_ls.h"

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

void	free_content(void *content)
{
	if (((t_content *) content)->path)
		free(((t_content *) content)->path);
	free(content);
}