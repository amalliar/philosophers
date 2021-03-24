/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:20:22 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 13:35:57 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void		*monitor_start(void *arg)
{
	t_sim_data		*sim_data;
	t_philo_status	*philo_stat_tab;
	int				i;

	sim_data = (t_sim_data *)arg;
	philo_stat_tab = sim_data->philo_stat_tab;
	while (1)
	{
		i = 0;
		while (i < sim_data->num_philos)
		{
			if (sim_data->time_to_die - get_timestamp() + \
				philo_stat_tab[i].last_time_eaten <= 0)
			{
				sim_data->sim_is_running = 0;
				pthread_mutex_lock(&sim_data->mtx_stdout);
				printf("%-8llu %d died\n", get_timestamp() - \
					sim_data->sim_start, philo_stat_tab[i].id);
				pthread_mutex_unlock(&sim_data->mtx_stdout);
				return (NULL);
			}
			++i;
		}
		usleep(100);
	}
}
