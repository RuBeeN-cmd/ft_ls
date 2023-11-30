#include "ft_ls.h"

void	invalid_option(char option)
{
	printf("ls: invalid option -- '%c'\n", option);
	exit(2);
}

void	unrecognized_option(char option[])
{
	printf("ls: unrecognized option '%s'\n", option);
	exit(2);
}

int	get_flags(char flags_str[])
{
	int	flags = 0;
	for (int i = 0; flags_str[i]; i++)
	{
		if (flags_str[i] == 'l')
			flags |= LONG;
		else if (flags_str[i] == 'R')
			flags |= RECURSIVE;
		else if (flags_str[i] == 'a')
			flags |= ALL;
		else if (flags_str[i] == 'r')
			flags |= REVERSE;
		else if (flags_str[i] == 't')
			flags |= TIME;
		else
			invalid_option(flags_str[i]);
	}
	return (flags);
}

int	parse_flags(int argc, char *argv[])
{
	int	flags = 0;
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1])
		{
			if (argv[i][1] == '-' && argv[i][2])
				unrecognized_option(argv[i]);
			else if (argv[i][1] != '-')
				flags |= get_flags(argv[i] + 1);
			argv[i][0] = 0;
		}
	}
	return (flags);
}