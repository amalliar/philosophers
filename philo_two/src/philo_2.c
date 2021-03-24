/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:23:41 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 12:25:17 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void			philo_take_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_wait(sim_data->sem_forks);
	if (sim_data->sim_is_running)
	{
		print_status(philo_status, "has taken a fork");
		print_status(philo_status, "has taken a fork");
	}
	else
		sem_post(sim_data->sem_forks);
}

void			philo_release_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_post(sim_data->sem_forks);
}
