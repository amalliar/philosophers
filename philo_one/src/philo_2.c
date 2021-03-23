/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:23:41 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/23 16:24:06 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int				philo_check_health(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	int64_t			time_to_live;

	sim_data = (t_sim_data *)philo_status->sim_data;
	time_to_live = sim_data->time_to_die - get_timestamp() + \
		philo_status->last_time_eaten;
	if (time_to_live <= 0)
	{
		pthread_mutex_lock(&sim_data->mtx_stdout_priority);
		if (sim_data->sim_is_running)
		{
			sim_data->sim_is_running = 0;
			pthread_mutex_lock(&sim_data->mtx_stdout_normal);
			printf("%-8llu %d died\n", get_timestamp() - sim_data->sim_start, \
				philo_status->id);
			pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		}
		pthread_mutex_unlock(&sim_data->mtx_stdout_priority);
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
	while (1)
	{
		pthread_mutex_lock(&sim_data->mtx_forks);
		if (!(is_dead = philo_check_health(philo_status)) && \
			sim_data->forks[philo_status->left_fork_idx] == AVAILABLE && \
			sim_data->forks[philo_status->right_fork_idx] == AVAILABLE)
		{
			sim_data->forks[philo_status->left_fork_idx] = TAKEN;
			print_status(philo_status, "has taken a fork");
			sim_data->forks[philo_status->right_fork_idx] = TAKEN;
			print_status(philo_status, "has taken a fork");
			pthread_mutex_unlock(&sim_data->mtx_forks);
			return (0);
		}
		else if (is_dead)
		{
			pthread_mutex_unlock(&sim_data->mtx_forks);
			return (1);
		}
		pthread_mutex_unlock(&sim_data->mtx_forks);
	}
}

void			philo_release_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sim_data->forks[philo_status->left_fork_idx] = AVAILABLE;
	sim_data->forks[philo_status->right_fork_idx] = AVAILABLE;
}
