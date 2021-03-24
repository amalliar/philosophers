/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:23:41 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 13:41:45 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void			philo_take_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	while (1)
	{
		pthread_mutex_lock(&sim_data->mtx_forks);
		if (!sim_data->sim_is_running)
		{
			pthread_mutex_unlock(&sim_data->mtx_forks);
			return ;
		}
		if (sim_data->forks[philo_status->left_fork_idx] == AVAILABLE && \
			sim_data->forks[philo_status->right_fork_idx] == AVAILABLE)
		{
			sim_data->forks[philo_status->left_fork_idx] = TAKEN;
			sim_data->forks[philo_status->right_fork_idx] = TAKEN;
			pthread_mutex_unlock(&sim_data->mtx_forks);
			print_status(philo_status, "has taken a fork");
			print_status(philo_status, "has taken a fork");
			return ;
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
