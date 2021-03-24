/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:28:12 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/27 11:06:13 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static void		init_philo_stat_tab(t_sim_data *sim_data)
{
	int			i;

	i = 0;
	while (i < sim_data->num_philos)
	{
		sim_data->philo_stat_tab[i].id = i + 1;
		sim_data->philo_stat_tab[i].left_fork_idx = i;
		sim_data->philo_stat_tab[i].right_fork_idx = (i == 0) ? \
			sim_data->num_philos - 1 : i - 1;
		sim_data->philo_stat_tab[i].cur_eat_cycles = 0;
		sim_data->philo_stat_tab[i].sim_data = sim_data;
		sim_data->forks[i] = AVAILABLE;
		++i;
	}
}

int				init_sim_data(t_sim_data *sim_data)
{
	int			i;

	i = sim_data->num_philos;
	if (!(sim_data->philo_stat_tab = malloc(i * sizeof(t_philo_status))) || \
		!(sim_data->philos = malloc(i * sizeof(pthread_t))) || \
		!(sim_data->forks = malloc(i * sizeof(int))))
		return (1);
	sim_data->sim_is_running = 1;
	sim_data->unfinished_philos = i;
	init_philo_stat_tab(sim_data);
	if (pthread_mutex_init(&sim_data->mtx_forks, NULL) || \
		pthread_mutex_init(&sim_data->mtx_stdout, NULL))
		return (1);
	return (0);
}

void			clear_sim_data(t_sim_data *sim_data)
{
	pthread_mutex_destroy(&sim_data->mtx_forks);
	pthread_mutex_destroy(&sim_data->mtx_stdout);
	free(sim_data->philo_stat_tab);
	free(sim_data->philos);
	free(sim_data->forks);
}

void			print_status(t_philo_status *philo_status, const char *msg)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	pthread_mutex_lock(&sim_data->mtx_stdout);
	if (sim_data->sim_is_running)
		printf("%-8lu %d %s\n", get_timestamp() - sim_data->sim_start, \
			philo_status->id, msg);
	pthread_mutex_unlock(&sim_data->mtx_stdout);
}
