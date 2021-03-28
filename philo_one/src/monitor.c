/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 13:20:22 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 20:36:40 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static int		check_health(t_philo_status *philo_status, uint64_t *min_ttl)
{
	t_sim_data		*sim_data;
	uint64_t		ttl;

	sim_data = (t_sim_data *)philo_status->sim_data;
	pthread_mutex_lock(&philo_status->mtx_last_time_eaten);
	ttl = get_microsec() - philo_status->last_time_eaten;
	ttl = ((uint64_t)sim_data->time_to_die * 1000 > ttl) ? \
		(uint64_t)sim_data->time_to_die * 1000 - ttl : 0;
	if (ttl == 0 && sim_data->sim_is_running)
	{
		sim_data->sim_is_running = 0;
		pthread_mutex_unlock(&philo_status->mtx_last_time_eaten);
		pthread_mutex_lock(&sim_data->mtx_stdout);
		printf("%-8lu %d died\n", (unsigned long)((get_microsec() - \
			sim_data->sim_start) / 1000), philo_status->id);
		pthread_mutex_unlock(&sim_data->mtx_stdout);
		return (1);
	}
	pthread_mutex_unlock(&philo_status->mtx_last_time_eaten);
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
