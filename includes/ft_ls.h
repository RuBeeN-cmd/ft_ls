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
#include <sys/ioctl.h>

#define MINORBITS        8
#define MINORMASK        ((1U << MINORBITS) - 1)
#define MAJOR(dev)        ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)        ((unsigned int) ((dev) & MINORMASK))

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
	char	*major;
	char	*minor;
	char	date[13];
	char	*name;
	char	*link;
}				t_line;

// args.c
void		parse_args(t_args *args, int argc, char *argv[]);
t_stat_list	*get_args_stats(t_args *args);
void		del_stat_lst(t_stat_list *lst);

// flags.c
void	parse_flags(t_args *args, int argc, char *argv[]);

// content.c
t_content	*init_content(struct stat stat_buf, char *name);
void		free_content(void *content);

// reg_file.c
void	treat_reg_file(t_args *args, t_stat_list *args_stats);

// remaining.c
void	treat_remaining(t_args args, t_stat_list *stats);

// list_folder.c
void	list_folder_content(char folder_name[], int flags, int multiple_args);
char	*get_file_path(char *folder_path, char *name);

// sort.c
void	sort_list(t_list *lst, int flags);

// print.c
void	show_list(t_list *lst, int flags, int is_reg);
int		contain_char(char c, char *set);

// column.c
void	show_column_format(t_list *lst);

// long_format.c
void	show_long_format(t_list *lst, int is_reg);
void	print_spaces(int n);

// quotes.c
int		need_quote(t_list *lst);
char	*change_name(char *name);

#endif