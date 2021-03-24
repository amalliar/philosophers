/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 11:37:37 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 18:29:24 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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
				sem_wait(sim_data->sem_stdout);
				printf("%-8llu %d died\n", get_timestamp() - \
					sim_data->sim_start, philo_stat_tab[i].id);
				sem_post(sim_data->sem_stdout);
				return (NULL);
			}
			++i;
		}
		usleep(1000);
	}
}
