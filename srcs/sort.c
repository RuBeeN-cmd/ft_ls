#include "ft_ls.h"

void	swap(t_list *l1, t_list *l2)
{
	void	*tmp = l1->content;
	l1->content = l2->content;
	l2->content = tmp;
}

int	is_less(t_list *l1, t_list *l2, int flags)
{
	if (!l1 || !l2)
		return (1);
	(void) flags;
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

t_list	*get_min(t_list *lst, int flags)
{
	if (!lst)
		return (NULL);
	t_list	*next_min = get_min(lst->next, flags);
	if (is_less(lst, next_min, flags))
		return (lst);
	return (next_min);
}

void	sort_list(t_list *lst, int flags)
{
	if (!lst)
		return ;
	t_list	*min = get_min(lst, flags);
	swap(lst, min);
	sort_list(lst->next, flags);
}
