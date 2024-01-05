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
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define	LONG		1 << 0
#define	RECURSIVE	1 << 1
#define	ALL			1 << 2
#define	REVERSE		1 << 3
#define	TIME		1 << 4

typedef struct	s_stat_lst
{
	struct s_stat_lst	*next;
	struct stat			stat_buf;	
}				t_stat_list;

typedef struct	s_args
{
	int			argc;
	char		**argv;
	int			flags;
	t_stat_list	*stats;
	int			show_title;
}				t_args;

typedef struct	s_content
{
	struct dirent	*dirent;
	struct stat		stat_buf;
	char			*name;
	char			*path;
}				t_content;
typedef struct	s_line
{
	char	type;
	char	perm[10];
	char	*nb_links;
	char	*owner_usr;
	char	*owner_group;
	char	*size;
	char	date[13];
	char	*name;
}				t_line;

void		parse_args(t_args *args, int argc, char *argv[]);
void		parse_flags(t_args *args, int argc, char *argv[]);
t_stat_list	*get_args_stats(t_args *args);
void		del_stat_lst(t_stat_list *lst);

void	sort_list(t_list *lst, int flags);
void	show_long_format(t_list *lst, int is_reg);
void	show_list(t_list *lst, int flags, int is_reg);
void	call_list_folder_foreach_folder(t_list *lst, int flags);

#endif