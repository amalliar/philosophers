/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 10:56:17 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 16:59:32 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

char			*ft_strchr(const char *str, int c)
{
	while (*str && *str != (char)c)
		++str;
	return ((*str == (char)c) ? (char *)str : NULL);
}

int				ft_atoi(const char *str)
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

static int		check_args(int argc, char **argv)
{
	int		i;
	char	*cur_arg;

	if (argc < 5 || argc > 6)
		return (printf("%s: Invalid number of arguments\n", argv[0]));
	i = 1;
	while (i < argc)
	{
		cur_arg = argv[i];
		if (*cur_arg == '+')
			++cur_arg;
		if (!*cur_arg)
			return (printf("%s: Empty argument value\n", argv[0]));
		while (*cur_arg)
			if (!ft_strchr("0123456789", *cur_arg++))
				return (printf("%s: Invalid argument format\n", argv[0]));
		++i;
	}
	return (0);
}

int				parse_args(int argc, char **argv, t_sim_data *sim_data)
{
	if (check_args(argc, argv))
		return (1);
	if ((sim_data->num_philos = ft_atoi(argv[1])) < 0 || \
		(sim_data->time_to_die = ft_atoi(argv[2])) < 0 || \
		(sim_data->time_to_eat = ft_atoi(argv[3])) < 0 || \
		(sim_data->time_to_sleep = ft_atoi(argv[4])) < 0 || \
		(argc == 6 && (sim_data->num_eat_cycles = ft_atoi(argv[5])) < 0))
		return (printf("%s: Argument too big for conversion\n", argv[0]));
	else if (sim_data->num_philos < 2)
		return (printf("%s: num_philos must be greater than 1\n", argv[0]));
	if (argc != 6)
		sim_data->num_eat_cycles = -1;
	else if (sim_data->num_eat_cycles == 0)
		return (printf("%s: num_eat_cycles must be greater than 0\n", argv[0]));
	if ((sim_data->time_to_think = sim_data->time_to_die - \
		sim_data->time_to_eat - sim_data->time_to_sleep) < 0)
		sim_data->time_to_think = 0;
	return (0);
}

uint64_t		get_microsec(void)
{
	struct timeval		cur;

	gettimeofday(&cur, NULL);
	return (cur.tv_sec * (uint64_t)1000000 + cur.tv_usec);
}
