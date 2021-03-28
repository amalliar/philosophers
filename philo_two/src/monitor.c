/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 11:37:37 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 20:38:00 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static int		check_health(t_philo_status *philo_status, uint64_t *min_ttl)
{
	t_sim_data		*sim_data;
	uint64_t		ttl;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_wait(sim_data->sem_last_time_eaten);
	ttl = get_microsec() - philo_status->last_time_eaten;
	ttl = ((uint64_t)sim_data->time_to_die * 1000 > ttl) ? \
		(uint64_t)sim_data->time_to_die * 1000 - ttl : 0;
	if (ttl == 0 && sim_data->sim_is_running)
	{
		sim_data->sim_is_running = 0;
		sem_post(sim_data->sem_last_time_eaten);
		sem_wait(sim_data->sem_stdout);
		printf("%-8lu %d died\n", (unsigned long)((get_microsec() - \
			sim_data->sim_start) / 1000), philo_status->id);
		sem_post(sim_data->sem_stdout);
		return (1);
	}
	sem_post(sim_data->sem_last_time_eaten);
	if (ttl < *min_ttl)
		*min_ttl = ttl;
	return (!sim_data->sim_is_running);
}

void			*monitor_start(void *arg)
{
	t_sim_data		*sim_data;
	int				i;
	uint64_t		min_ttl;

	sim_data = (t_sim_data *)arg;
	while (1)
	{
		min_ttl = sim_data->time_to_die;
		i = 0;
		while (i < sim_data->num_philos)
			if (check_health(&sim_data->philo_stat_tab[i++], &min_ttl))
				return (NULL);
		microsleep(min_ttl);
	}
}
