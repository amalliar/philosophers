/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 11:37:37 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 20:40:02 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static void		check_health(t_philo_status *philo_status, uint64_t *ttl)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_wait(sim_data->sem_last_time_eaten);
	*ttl = get_microsec() - philo_status->last_time_eaten;
	*ttl = ((uint64_t)sim_data->time_to_die * 1000 > *ttl) ? \
		(uint64_t)sim_data->time_to_die * 1000 - *ttl : 0;
	if (*ttl == 0)
	{
		sem_wait(sim_data->sem_stdout);
		printf("%-8lu %d died\n", (unsigned long)((get_microsec() - \
			sim_data->sim_start) / 1000), philo_status->id);
		kill(0, 9);
	}
	sem_post(sim_data->sem_last_time_eaten);
}

void			*monitor_start(void *arg)
{
	t_sim_data		*sim_data;
	t_philo_status	*philo_status;
	uint64_t		ttl;

	philo_status = (t_philo_status *)arg;
	sim_data = (t_sim_data *)philo_status->sim_data;
	while (1)
	{
		ttl = sim_data->time_to_die;
		check_health(philo_status, &ttl);
		microsleep(ttl);
	}
}
