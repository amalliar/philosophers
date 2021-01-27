/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 10:56:17 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/19 02:42:37 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <limits.h>
#include "philo_one.h"

char		*ft_strchr(const char *str, int c)
{
	while (*str && *str != (char)c)
		++str;
	return ((*str == (char)c) ? (char *)str : NULL);
}

int			ft_atoi(const char *str)
{
	long long	num;
	int			negative;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		++str;
	if (*str == '-' || *str == '+')
	{
		negative = (*str == '-') ? 1 : 0;
		++str;
	}
	else
		negative = 0;
	num = 0;
	while (num <= INT_MAX && *str >= '0' && *str <= '9')
	{
		num = num * 10 + *str - '0';
		++str;
	}
	if (negative)
		return ((-num < INT_MIN || (*str >= '0' && *str <= '9')) ? 0 : -num);
	return ((num > INT_MAX || (*str >= '0' && *str <= '9')) ? -1 : num);
}

int			check_args(int argc, char **argv)
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
		if (*cur_arg == '+')
			++cur_arg;
		if (!*cur_arg)
		{
			printf("%s: Empty argument value\n", argv[0]);
			return (1);
		}
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

t_run_opts	*parse_args(int argc, char **argv)
{
	t_run_opts		*run_opts;

	if (check_args(argc, argv))
		return (NULL);
	if (!(run_opts = malloc(sizeof(t_run_opts))))
		return (NULL);
	if ((run_opts->num_philos = ft_atoi(argv[1])) < 2)
	{
		printf("%s: Number of philosophers must be greater than 1", argv[0]);
		return (NULL);
	}
	run_opts->time_to_die = ft_atoi(argv[2]);
	run_opts->time_to_eat = ft_atoi(argv[3]);
	run_opts->time_to_sleep = ft_atoi(argv[4]);
	run_opts->num_eat_cycles = (argc == 6) ? ft_atoi(argv[5]) : -1;
	return (run_opts);
}
