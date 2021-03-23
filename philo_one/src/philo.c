/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:22:32 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/23 14:48:07 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int				philo_check_health(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	int64_t			time_to_live;

	sim_data = (t_sim_data *)philo_status->sim_data;
	time_to_live = sim_data->time_to_die - get_timestamp() + philo_status->last_time_eaten;
	if (time_to_live <= 0)
	{
		pthread_mutex_lock(&sim_data->mtx_stdout_priority);
		if (sim_data->sim_is_running)
		{
			sim_data->sim_is_running = 0;
			pthread_mutex_lock(&sim_data->mtx_stdout_normal);
			printf("%-8llu %d died\n", get_timestamp() - sim_data->sim_start, philo_status->id);
			pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		}
		pthread_mutex_unlock(&sim_data->mtx_stdout_priority);
		return (1);
	}
	else if (!sim_data->sim_is_running)
		return (1);
	return (0);
}

void			philo_take_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	pthread_mutex_lock(&sim_data->mtx_forks[philo_status->left_fork_idx]);
	if (philo_check_health(philo_status))
	{
		pthread_mutex_unlock(&sim_data->mtx_forks[philo_status->left_fork_idx]);
		return ;
	}
	pthread_mutex_lock(&sim_data->mtx_forks[philo_status->right_fork_idx]);
}

void			philo_release_forks(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	pthread_mutex_unlock(&sim_data->mtx_forks[philo_status->left_fork_idx]);
	pthread_mutex_unlock(&sim_data->mtx_forks[philo_status->right_fork_idx]);
}

int				philo_eat(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	philo_take_forks(philo_status);
	if (philo_check_health(philo_status))
	{
		philo_release_forks(philo_status);
		return (1);
	}
	pthread_mutex_lock(&sim_data->mtx_stdout_normal);
	if (sim_data->sim_is_running)
		printf("%-8llu %d is eating\n", get_timestamp() - sim_data->sim_start, philo_status->id);
	pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
	usleep(sim_data->time_to_eat * 1000);
	philo_status->last_time_eaten = get_timestamp();
	if (++philo_status->cur_eat_cycles == sim_data->num_eat_cycles)
		if (--sim_data->unfinished_philos == 0)
			sim_data->sim_is_running = 0;
	philo_release_forks(philo_status);
	return (0);
}

int				philo_sleep(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	pthread_mutex_lock(&sim_data->mtx_stdout_normal);
	if (sim_data->sim_is_running)
		printf("%-8llu %d is sleeping\n", get_timestamp() - sim_data->sim_start, philo_status->id);
	pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
	usleep(sim_data->time_to_sleep * 1000);
	return (philo_check_health(philo_status));
}

int				philo_think(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;
	int64_t			time_to_live;

	sim_data = (t_sim_data *)philo_status->sim_data;
	pthread_mutex_lock(&sim_data->mtx_stdout_normal);
	if (sim_data->sim_is_running)
		printf("%-8llu %d is thinking\n", get_timestamp() - sim_data->sim_start, philo_status->id);
	pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
	time_to_live = sim_data->time_to_die - get_timestamp() + philo_status->last_time_eaten;
	if (time_to_live > 0)
		usleep(time_to_live * 400);
	return (philo_check_health(philo_status));
}

void			*philo_start(void *arg)
{
	t_philo_status		*philo_status;

	philo_status = (t_philo_status *)arg;
	while (1)
	{
		if (philo_eat(philo_status) || philo_sleep(philo_status) || \
			philo_think(philo_status))
			return (NULL);
	}
}
