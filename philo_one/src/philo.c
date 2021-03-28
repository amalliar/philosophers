/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:22:32 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 12:39:03 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static void		philo_eat(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	philo_take_forks(philo_status);
	philo_status->last_time_eaten = get_microsec();
	print_status(philo_status, "is eating");
	usleep(sim_data->time_to_eat * 1000);
	if (++philo_status->cur_eat_cycles == sim_data->num_eat_cycles)
		if (--sim_data->unfinished_philos == 0)
			sim_data->sim_is_running = 0;
	philo_release_forks(philo_status);
}

static void		philo_sleep(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	print_status(philo_status, "is sleeping");
	usleep(sim_data->time_to_sleep * 1000);
}

static void		philo_think(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	uint64_t		ttl;

	sim_data = (t_sim_data *)philo_status->sim_data;
	print_status(philo_status, "is thinking");
	ttl = get_microsec() - philo_status->last_time_eaten;
	ttl = ((uint64_t)sim_data->time_to_die * 1000 > ttl) ? \
		(uint64_t)sim_data->time_to_die * 1000 - ttl : 0;
	if (ttl > (uint64_t)sim_data->time_to_think * 1000)
		usleep(sim_data->time_to_think * 1000);
}

void			*philo_start(void *arg)
{
	t_philo_status		*philo_status;
	t_sim_data			*sim_data;

	philo_status = (t_philo_status *)arg;
	sim_data = (t_sim_data *)philo_status->sim_data;
	while (1)
	{
		philo_eat(philo_status);
		philo_sleep(philo_status);
		philo_think(philo_status);
		if (!sim_data->sim_is_running)
			return (NULL);
	}
}
