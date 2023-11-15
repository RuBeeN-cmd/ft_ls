#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define	LONG		0
#define	RECURSIVE	1
#define	ALL			2
#define	REVERSE		3
#define	TIME		4

int	get_flags(int *argc_ptr, char **argv_ptr[])
{
	int flags = 0;
	while (*argc_ptr && **argv_ptr[0] == '-')
	{
		for (int j = 0; **argv_ptr[j]; j++)
		{
				if (**argv_ptr[j] == 'l')
					flags |= 1 << LONG;
				if (**argv_ptr[j] == 'R')
					flags |= 1 << RECURSIVE;
				if (**argv_ptr[j] == 'a')
					flags |= 1 << ALL;
				if (**argv_ptr[j] == 'r')
					flags |= 1 << REVERSE;
				if (**argv_ptr[j] == 't')
					flags |= 1 << TIME;
		}
		*argc_ptr = *argc_ptr - 1;
		*argv_ptr = *argv_ptr + 1;
	}
	return (flags);
}

void	list_files(int argc, char *argv[])
{
	if (!argc)
		return ;
	DIR *dir;
	if (!(dir = opendir(*argv)))
		printf("ft_ls: %s: No such file or directory", *argv);
	else
	{
		struct dirent	*dirent;
		while ((dirent = readdir(dir)))
		{
			printf("%s\n", dirent->d_name);
		}
		if (closedir(dir) == -1)
			printf("Closedir error\n");
	}
	list_files(argc - 1, argv + 1);
}

int main(int argc, char *argv[])
{
	// int		flags = get_flags(&argc, &argv);
	list_files(argc - 1, argv + 1);
	return (0);
}
