#include "ft_ls.h"

int main(int argc, char *argv[])
{
	t_args		args;
	t_stat_list	*stats;
	int			ret = 0;
	
	args = parse_args(argc - 1, argv + 1);
	if (!args.argc)
		return (list_folder_content(".", args.flags, 0));
	ret = get_args_stats(&args, &stats);
	treat_reg_file(&args, stats);
	int	err;
	if ((err = treat_remaining(args, stats)) > ret)
		ret = err;
	del_stat_lst(stats);
	return (ret);
}