/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 08:04:46 by amalliar          #+#    #+#             */
/*   Updated: 2021/01/23 11:30:01 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "philo_one.h"

typedef struct		s_run_opts
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat_cycles;
}					t_run_opts;

int				check_args(int argc, char **argv)
{
	int		i;
	char	*cur_arg;

	if (argc < 5 || argc > 6)
	{
		printf("%s: Invalid number of arguments\n", argv[0]);
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		cur_arg = argv[i];
		while (*cur_arg)
			if (!ft_strchr("0123456789", *cur_arg++))
			{
				printf("%s: Invalid argument format\n", argv[0]);
				return (1);
			}
		++i;
	}
	return (0);
}

t_run_opts		*parse_args(int argc, char **argv)
{
	t_run_opts		*run_opts;

	if (check_args(argc, argv))
		return (NULL);
	if (!(run_opts = malloc(sizeof(t_run_opts))))
		return (NULL);
	run_opts->num_philos = ft_atoi(argv[1]);
	run_opts->time_to_die = ft_atoi(argv[2]);
	run_opts->time_to_eat = ft_atoi(argv[3]);
	run_opts->time_to_sleep = ft_atoi(argv[4]);
	run_opts->num_eat_cycles = (argc == 6) ? ft_atoi(argv[5]) : 0;
	return (run_opts);
}

int				main(int argc, char **argv)
{
	t_run_opts		*run_opts;

	if (!(run_opts = parse_args(argc, argv)))
		return (1);
	return (0);
}
