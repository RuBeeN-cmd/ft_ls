#include "ft_ls.h"

void	swap(t_list *l1, t_list *l2)
{
	void	*tmp = l1->content;
	l1->content = l2->content;
	l2->content = tmp;
}

int	is_less_by_name(t_list *l1, t_list *l2)
{
	if (!l1 || !l2)
		return (1);
	char	*n1 = ((struct dirent *) l1->content)->d_name;
	char	*n2 = ((struct dirent *) l2->content)->d_name;
	int i = 0;
	int j = 0;
	while (n1[i] == '.' || n1[i] == '_')
		i++;
	while (n2[j] == '.' || n2[j] == '_')
		j++;
	while (n1[i] && ft_tolower(n1[i]) == ft_tolower(n2[j]))
	{
		i++;
		j++;
		while (n1[i] == '.' || n1[i] == '_')
			i++;
		while (n2[j] == '.' || n2[j] == '_')
			j++;
	}
	if (n1[i] || n2[j])
		return (ft_tolower(n1[i]) < ft_tolower(n2[j]));
	i = 0;
	j = 0;
	while (n1[i] == '.' || n1[i] == '_')
		i++;
	while (n2[j] == '.' || n2[j] == '_')
		j++;
	while (n1[i] && n1[i] == n2[j])
	{
		i++;
		j++;
		while (n1[i] == '.' || n1[i] == '_')
			i++;
		while (n2[j] == '.' || n2[j] == '_')
			j++;
	}
	if (n1[i] || n2[j])
		return (n1[i] > n2[j]);
	i = 0;
	while (n1[i] && n1[i] == n2[i])
		i++;
	return (n1[i] < n2[i]);
}

int	is_less_by_time(t_list *l1, t_list *l2)
{
	if (!l1 || !l2)
		return (1);
	return (1);
}

t_list	*get_min(t_list *lst, int flags)
{
	if (!lst)
		return (NULL);
	if (!lst->next)
		return (lst);
	t_list	*next_min = get_min(lst->next, flags);
	int is_less = 0;
	if (flags & TIME)
		is_less = is_less_by_time(lst, next_min);
	else
		is_less = is_less_by_name(lst, next_min);
	if ((!(flags & REVERSE) && is_less) || ((flags & REVERSE) && !is_less))
		return (lst);
	return (next_min);
}

void	sort_list(t_list *lst, int flags)
{
	if (!lst->next)
		return ;
	t_list	*min = get_min(lst, flags);
	swap(lst, min);
	sort_list(lst->next, flags);
}
