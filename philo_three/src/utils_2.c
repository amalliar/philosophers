/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:28:12 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 19:56:53 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int				init_sim_data(t_sim_data *sim_data)
{
	int			i;

	i = sim_data->num_philos;
	if (!(sim_data->philos = malloc(i * sizeof(pid_t))))
		return (1);
	sem_unlink("/sem_forks");
	sem_unlink("/sem_stdout");
	sem_unlink("/sem_last_time_eaten");
	sem_unlink("/sem_unfinished_philos");
	if ((sim_data->sem_forks = \
		sem_open("/sem_forks", O_CREAT, 0644, i / 2)) == SEM_FAILED || \
		(sim_data->sem_stdout = \
		sem_open("/sem_stdout", O_CREAT, 0644, 1)) == SEM_FAILED || \
		(sim_data->sem_last_time_eaten = \
		sem_open("/sem_last_time_eaten", O_CREAT, 0644, 1)) == SEM_FAILED || \
		(sim_data->sem_unfinished_philos = \
		sem_open("/sem_unfinished_philos", O_CREAT, 0644, 0)) == SEM_FAILED)
		return (1);
	return (0);
}

void			clear_sim_data(t_sim_data *sim_data)
{
	sem_close(sim_data->sem_forks);
	sem_close(sim_data->sem_stdout);
	sem_close(sim_data->sem_last_time_eaten);
	sem_close(sim_data->sem_unfinished_philos);
	sem_unlink("/sem_forks");
	sem_unlink("/sem_stdout");
	sem_unlink("/sem_last_time_eaten");
	sem_unlink("/sem_unfinished_philos");
	free(sim_data->philos);
}

void			print_status(t_philo_status *philo_status, const char *msg)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_wait(sim_data->sem_stdout);
	printf("%-8lu %d %s\n", (get_microsec() - sim_data->sim_start) / 1000, \
		philo_status->id, msg);
	sem_post(sim_data->sem_stdout);
}

void			microsleep(uint64_t microsec)
{
	uint64_t	start;

	start = get_microsec();
	while (get_microsec() - start < microsec)
		usleep(1);
}
