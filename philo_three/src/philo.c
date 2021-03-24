/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:22:32 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 18:17:27 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static void		philo_think(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	int64_t			time_to_live;

	sim_data = (t_sim_data *)philo_status->sim_data;
	print_status(philo_status, "is thinking");
	time_to_live = sim_data->time_to_die - get_timestamp() + \
		philo_status->last_time_eaten;
	if (time_to_live > 0)
		usleep(time_to_live * 400);
}

static void		philo_eat(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	philo_take_forks(philo_status);
	philo_status->last_time_eaten = get_timestamp();
	print_status(philo_status, "is eating");
	if (++philo_status->cur_eat_cycles == sim_data->num_eat_cycles)
		if (--sim_data->unfinished_philos == 0)
			sim_data->sim_is_running = 0;
	usleep(sim_data->time_to_eat * 1000);
	philo_release_forks(philo_status);
}

static void		philo_sleep(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	print_status(philo_status, "is sleeping");
	usleep(sim_data->time_to_sleep * 1000);
}

void			*philo_start(void *arg)
{
	t_philo_status		*philo_status;
	t_sim_data			*sim_data;

	philo_status = (t_philo_status *)arg;
	sim_data = (t_sim_data *)philo_status->sim_data;
	while (1)
	{
		philo_think(philo_status);
		philo_eat(philo_status);
		philo_sleep(philo_status);
		if (!sim_data->sim_is_running)
			return (NULL);
	}
}
