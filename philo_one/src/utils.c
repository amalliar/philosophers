/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 10:56:17 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/23 13:17:28 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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

int				parse_args(int argc, char **argv, t_sim_data *sim_data)
{
	if (check_args(argc, argv))
		return (1);
	if ((sim_data->num_philos = ft_atoi(argv[1])) < 2)
	{
		printf("%s: Number of philosophers must be greater than 1", argv[0]);
		return (1);
	}
	sim_data->time_to_die = ft_atoi(argv[2]);
	sim_data->time_to_eat = ft_atoi(argv[3]);
	sim_data->time_to_sleep = ft_atoi(argv[4]);
	sim_data->num_eat_cycles = (argc == 6) ? ft_atoi(argv[5]) : -1;
	return (0);
}

uint64_t		get_timestamp(void)
{
	struct timeval		cur;

	gettimeofday(&cur, NULL);
	return ((uint64_t)cur.tv_sec * 1000 + cur.tv_usec / 1000);
}

int				init_sim_data(t_sim_data *sim_data)
{
	int			i;

	i = sim_data->num_philos;
	if (!(sim_data->philo_stat_tab = malloc(i * sizeof(t_philo_status))) || \
		!(sim_data->threads = malloc(i * sizeof(pthread_t))) || \
		!(sim_data->mtx_forks = malloc(i * sizeof(pthread_mutex_t))))
		return (1);
	sim_data->sim_start = get_timestamp();
	sim_data->sim_is_running = 1;
	sim_data->unfinished_philos = sim_data->num_philos;
	i = 0;
	while (i < sim_data->num_philos)
	{
		sim_data->philo_stat_tab[i].id = i + 1;
		sim_data->philo_stat_tab[i].left_fork_idx = i;
		sim_data->philo_stat_tab[i].right_fork_idx = (i == 0) ? \
			sim_data->num_philos - 1 : i - 1;
		sim_data->philo_stat_tab[i].cur_eat_cycles = 0;
		sim_data->philo_stat_tab[i].last_time_eaten = sim_data->sim_start;
		sim_data->philo_stat_tab[i].sim_data = sim_data;
		++i;
	}
	if (pthread_mutex_init(&sim_data->mtx_stdout_normal, NULL) || \
		pthread_mutex_init(&sim_data->mtx_stdout_priority, NULL))
		return (1);
	i = 0;
	while (i < sim_data->num_philos)
	{
		if (pthread_mutex_init(sim_data->mtx_forks + i, NULL))
			return (1);
		++i;
	}
	return (0);
}

void			clear_sim_data(t_sim_data *sim_data)
{
	int		i;

	pthread_mutex_destroy(&sim_data->mtx_stdout_normal);
	pthread_mutex_destroy(&sim_data->mtx_stdout_priority);
	i = 0;
	while (i < sim_data->num_philos)
	{
		pthread_mutex_destroy(sim_data->mtx_forks + i);
		++i;
	}
	free(sim_data->philo_stat_tab);
	free(sim_data->threads);
	free(sim_data->mtx_forks);
}
