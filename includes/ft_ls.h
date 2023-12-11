#ifndef FT_LS_H
#define FT_LS_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libft.h>
#include <pwd.h>
#include <grp.h>

#define	LONG		1 << 0
#define	RECURSIVE	1 << 1
#define	ALL			1 << 2
#define	REVERSE		1 << 3
#define	TIME		1 << 4

typedef struct	s_args
{
	int		argc;
	char	**argv;
	int		flags;
}				t_args;

int		parse_flags(char argv[]);

#endif