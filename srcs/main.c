#include "ft_ls.h"

void	parse_args(t_args *args, int argc, char *argv[])
{
	args->flags = 0;
	args->argc = 0;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1])
			args->flags |= parse_flags(argv[i]);
		else
			args->argc++;
	}
	args->argv = argv;
}

t_list	*fill_list(DIR *dir, int flags)
{
	t_list	*lst;

	struct dirent	*dirent;
	if (!(dirent = readdir(dir)))
		return (NULL);
	if (dirent->d_name[0] == '.' && !(flags & ALL))
		return (fill_list(dir, flags));
	lst = ft_lstnew(dirent);
	lst->next = fill_list(dir, flags);
	return (lst);
}

void	list_folder_content(char folder_name[], int flags)
{
	t_list	*lst;
	DIR		*dir;
	if ((dir = opendir(folder_name)))
	{
		lst = fill_list(dir, flags);
		sort_list(lst, flags);
		show_list(lst, flags);
		if (closedir(dir) == -1)
			printf("Closedir error\n");
	}
	else
		printf("Opendir error\n");
}

int main(int argc, char *argv[])
{
	t_args	args;
	parse_args(&args, argc - 1, argv + 1);
	if (!args.argc)
		list_folder_content(".", args.flags);
	else
	{
		for (int i = 0; args.argc; i++)
		{
			if (args.argv[i][0])
			{
				list_folder_content(args.argv[i], args.flags);
				args.argc--;
			}
		}
	}
	return (0);
}
