/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:23:41 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 16:18:32 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void			philo_take_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	while (1)
		if (sim_data->forks[philo_status->l_fork_idx] == FS_AVAILABLE && \
			sim_data->forks[philo_status->r_fork_idx] == FS_AVAILABLE)
		{
			pthread_mutex_lock(&sim_data->mtx_forks);
			if (sim_data->forks[philo_status->l_fork_idx] == FS_AVAILABLE && \
				sim_data->forks[philo_status->r_fork_idx] == FS_AVAILABLE)
			{
				sim_data->forks[philo_status->l_fork_idx] = FS_TAKEN;
				sim_data->forks[philo_status->r_fork_idx] = FS_TAKEN;
				pthread_mutex_unlock(&sim_data->mtx_forks);
				print_status(philo_status, "has taken a fork");
				print_status(philo_status, "has taken a fork");
				return ;
			}
			pthread_mutex_unlock(&sim_data->mtx_forks);
		}
		else
			microsleep(10);
}

void			philo_release_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sim_data->forks[philo_status->l_fork_idx] = FS_AVAILABLE;
	sim_data->forks[philo_status->r_fork_idx] = FS_AVAILABLE;
}
