#include "ft_ls.h"

int	is_bracket(char *n)
{
	if (!n)
		return (0);
	return ((n[0] == '{' || n[0] == '}') && !n[1]);
}

int	is_dieze(char *n)
{
	if (!n)
		return (0);
	return (n[0] == '#');
}

int	str_contain(char *n, char *set)
{
	if (!n || !set)
		return (0);
	for (int i = 0; n[i]; i++)
		for (int j = 0; set[j]; j++)
			if (n[i] == set[j])
				return (1);
	return (0);
}

int	contain_char(char c, char *set)
{
	if (!set)
		return (0);
	for (int i = 0; set[i]; i++)
		if (c == set[i])
			return (1);
	return (0);
}

int	does_have_quote(char *name)
{
	if (!name)
		return (0);
	if (is_bracket(name))
		return (1);
	if (is_dieze(name))
		return (1);
	if ((str_contain(name, " !$^&*()=<>?;[]~\"")))
		return (1);
	if ((str_contain(name, "\'")))
		return (2);
	return (0);
}

char	*add_around(char **src, char c)
{
	int 	len = ft_strlen(*src);
	char	*dst = malloc(len + 3);
	dst[0] = c;
	dst[len + 1] = c;
	dst[len + 2] = 0;
	ft_memcpy(dst + 1, *src, len);
	return (dst);
}

char	*add_before(char **src, char c)
{
	int 	len = ft_strlen(*src);
	char	*dst = malloc(len + 2);
	dst[0] = c;
	dst[len + 1] = 0;
	ft_memcpy(dst + 1, *src, len);
	return (dst);
}

void	change_name(t_list *lst)
{
	char	*new;

	while (lst)
	{
		int quote = does_have_quote(((t_content *) lst->content)->name);
		if (quote == 1)
			new = add_around(&((t_content *) lst->content)->name, '\'');
		else if (quote == 2)
			new = add_around(&((t_content *) lst->content)->name, '\"');
		else
			new = add_before(&((t_content *) lst->content)->name, ' ');
		free(((t_content *) lst->content)->name);
		((t_content *) lst->content)->name = new;
		lst = lst->next;
	}
}

int	need_quote(t_list *lst)
{
	int	ret;

	ret = 0;
	while (lst)
	{
		if (does_have_quote(((t_content *) lst->content)->name))
			ret = 1;
		lst = lst->next;
	}
	return (ret);
}

void	show_list(t_list *lst, int flags, int is_reg)
{
	if (need_quote(lst))
		change_name(lst);
	if (flags & LONG)
		show_long_format(lst, is_reg);
	else
		show_column_format(lst);
}