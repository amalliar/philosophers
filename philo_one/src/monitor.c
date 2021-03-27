/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:20:22 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/27 12:47:20 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static void		*sim_stop(t_sim_data *sim_data, int id)
{
	sim_data->sim_is_running = 0;
	pthread_mutex_lock(&sim_data->mtx_stdout);
	printf("%-8lu %d died\n", get_timestamp() - \
		sim_data->sim_start, id);
	pthread_mutex_unlock(&sim_data->mtx_stdout);
	return (NULL);
}

void			*monitor_start(void *arg)
{
	t_sim_data		*sim_data;
	t_philo_status	*philo_stat_tab;
	int				i;
	int				ttl;
	int				min_ttl;

	sim_data = (t_sim_data *)arg;
	philo_stat_tab = sim_data->philo_stat_tab;
	while (1)
	{
		min_ttl = sim_data->time_to_die;
		i = 0;
		while (i < sim_data->num_philos)
		{
			ttl = sim_data->time_to_die - get_timestamp() + \
				philo_stat_tab[i].last_time_eaten;
			if (ttl <= 0 && sim_data->sim_is_running)
				return (sim_stop(sim_data, i + 1));
			min_ttl = (ttl < min_ttl) ? ttl : min_ttl;
			++i;
		}
		if (!sim_data->sim_is_running)
			return (NULL);
		usleep(min_ttl * 1000);
	}
}
