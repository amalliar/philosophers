/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:28:12 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 20:34:11 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static int		init_philo_stat_tab(t_sim_data *sim_data)
{
	int			i;

	i = 0;
	while (i < sim_data->num_philos)
	{
		sim_data->philo_stat_tab[i].id = i + 1;
		sim_data->philo_stat_tab[i].l_fork_idx = i;
		sim_data->philo_stat_tab[i].r_fork_idx = (i == 0) ? \
			sim_data->num_philos - 1 : i - 1;
		sim_data->philo_stat_tab[i].cur_eat_cycles = 0;
		sim_data->philo_stat_tab[i].sim_data = sim_data;
		sim_data->forks[i] = FS_AVAILABLE;
		if (pthread_mutex_init(\
			&sim_data->philo_stat_tab[i].mtx_last_time_eaten, NULL))
			return (1);
		++i;
	}
	return (0);
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
	if (init_philo_stat_tab(sim_data) || \
		pthread_mutex_init(&sim_data->mtx_forks, NULL) || \
		pthread_mutex_init(&sim_data->mtx_stdout, NULL))
		return (1);
	return (0);
}

void			clear_sim_data(t_sim_data *sim_data)
{
	int		i;

	pthread_mutex_destroy(&sim_data->mtx_forks);
	pthread_mutex_destroy(&sim_data->mtx_stdout);
	i = 0;
	while (i < sim_data->num_philos)
		pthread_mutex_destroy(\
			&sim_data->philo_stat_tab[i++].mtx_last_time_eaten);
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
		printf("%-8lu %d %s\n", (unsigned long)((get_microsec() - \
			sim_data->sim_start) / 1000), philo_status->id, msg);
	pthread_mutex_unlock(&sim_data->mtx_stdout);
}

void			microsleep(uint64_t microsec)
{
	uint64_t	start;

	start = get_microsec();
	while (get_microsec() - start < microsec)
		usleep(1);
}
