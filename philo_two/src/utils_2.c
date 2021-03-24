/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 16:28:12 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 12:46:39 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static void		init_philo_stat_tab(t_sim_data *sim_data)
{
	int			i;

	i = 0;
	while (i < sim_data->num_philos)
	{
		sim_data->philo_stat_tab[i].id = i + 1;
		sim_data->philo_stat_tab[i].cur_eat_cycles = 0;
		sim_data->philo_stat_tab[i].last_time_eaten = sim_data->sim_start;
		sim_data->philo_stat_tab[i].sim_data = sim_data;
		++i;
	}
}

int				init_sim_data(t_sim_data *sim_data)
{
	int			i;

	i = sim_data->num_philos;
	if (!(sim_data->philo_stat_tab = malloc(i * sizeof(t_philo_status))) || \
		!(sim_data->philos = malloc(i * sizeof(pthread_t))))
		return (1);
	sim_data->sim_start = get_timestamp();
	sim_data->sim_is_running = 1;
	sim_data->unfinished_philos = i;
	init_philo_stat_tab(sim_data);
	sem_unlink("sem_forks");
	sem_unlink("sem_stdout");
	if ((sim_data->sem_forks = \
		sem_open("sem_forks", O_CREAT, 0644, i / 2)) == SEM_FAILED || \
		(sim_data->sem_stdout = \
		sem_open("sem_stdout", O_CREAT, 0644, 1)) == SEM_FAILED)
		return (1);
	return (0);
}

void			clear_sim_data(t_sim_data *sim_data)
{
	sem_close(sim_data->sem_forks);
	sem_close(sim_data->sem_stdout);
	sem_unlink("sem_forks");
	sem_unlink("sem_stdout");
	free(sim_data->philo_stat_tab);
	free(sim_data->philos);
}

void			print_status(t_philo_status *philo_status, const char *msg)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	sem_wait(sim_data->sem_stdout);
	if (sim_data->sim_is_running)
		printf("%-8llu %d %s\n", get_timestamp() - sim_data->sim_start, \
			philo_status->id, msg);
	sem_post(sim_data->sem_stdout);
}
