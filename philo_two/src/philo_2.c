/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:23:41 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 11:08:38 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int				philo_check_health(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	int64_t			time_to_live;

	sim_data = (t_sim_data *)philo_status->sim_data;
	time_to_live = sim_data->time_to_die - get_timestamp() + \
		philo_status->last_time_eaten;
	if (time_to_live <= 0)
	{
		sem_wait(sim_data->sem_stdout_priority);
		if (sim_data->sim_is_running)
		{
			sim_data->sim_is_running = 0;
			sem_wait(sim_data->sem_stdout_normal);
			printf("%-8llu %d died\n", get_timestamp() - sim_data->sim_start, \
				philo_status->id);
			sem_post(sim_data->sem_stdout_normal);
		}
		sem_post(sim_data->sem_stdout_priority);
		return (1);
	}
	else if (!sim_data->sim_is_running)
		return (1);
	return (0);
}

int				philo_take_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	int				is_dead;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_wait(sim_data->sem_forks);
	if (!(is_dead = philo_check_health(philo_status)))
	{
		print_status(philo_status, "has taken a fork");
		print_status(philo_status, "has taken a fork");
		return (0);
	}
	else
	{
		sem_post(sim_data->sem_forks);
		return (1);
	}
}

void			philo_release_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_post(sim_data->sem_forks);
}
