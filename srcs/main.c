#include "ft_ls.h"

int main(int argc, char *argv[])
{
	t_args		args;
	t_stat_list	*stats;
	
	parse_args(&args, argc - 1, argv + 1);
	if (!args.argc)
		list_folder_content(".", args.flags, 0);
	else
	{
		stats = get_args_stats(&args);
		treat_reg_file(&args, stats);
		treat_remaining(args, stats);
		del_stat_lst(stats);
	}
	return (0);
}
