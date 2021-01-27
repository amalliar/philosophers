/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 08:04:46 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/19 02:46:24 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void			*philo_start(void *arg)
{
	t_philo_status	*philo_status;
	t_sim_data		*sim_data;

	philo_status = (t_philo_status *)arg;
	sim_data = (t_sim_data *)philo_status->sim_data;
	while (1)
	{
		srand(time(NULL));
		usleep(1000000);
		pthread_mutex_lock(&sim_data->mtx_stdout_normal);
		struct timeval	cur_time;
		gettimeofday(&cur_time, NULL);
		printf("%f %d says hello!\n", (float)(cur_time.tv_usec - (sim_data->tv_sim_start).tv_usec) / 1000, philo_status->id);
		pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		// get right fork (random OR philo_id % 2 to avoid dead locks)
		// check if dead
		// get left fork
		// check if dead
		// eat
		// --num_eat_cycles_remaining
		// release forks
		// sleep
		// check if dead
		// think
		// check if dead
	}
}

int				start_threads(t_sim_data *sim_data)
{
	int		i;

	i = 0;
	while (i < sim_data->run_opts->num_philos)
	{
		if (pthread_create(&sim_data->threads[i], NULL, philo_start, &sim_data->philo_stat_tab[i]))
			return (1);
		++i;
	}
	return (0);
}


int				init_sim_data(t_sim_data *sim_data)
{
	int			i;

	// Allocate resources.
	i = sim_data->run_opts->num_philos;
	if (!(sim_data->philo_stat_tab = malloc(i * sizeof(t_philo_status))) || \
		!(sim_data->threads = malloc(i * sizeof(pthread_t))) || \
		!(sim_data->mtx_forks = malloc(i * sizeof(pthread_mutex_t))))
		return (1);

	gettimeofday(&sim_data->tv_sim_start, NULL);
	sim_data->sim_is_running = 1;

	// Init philo_stat_tab.
	i = 0;
	while (i < sim_data->run_opts->num_philos)
	{
		(sim_data->philo_stat_tab)[i].id = i + 1;
		(sim_data->philo_stat_tab)[i].cur_eat_cycles = 0;
		(sim_data->philo_stat_tab)[i].tv_last_time_eaten = sim_data->tv_sim_start;
		(sim_data->philo_stat_tab)[i].sim_data = sim_data;
		++i;
	}

	// Init mutexes.
	if (pthread_mutex_init(&sim_data->mtx_stdout_normal, NULL) || \
		pthread_mutex_init(&sim_data->mtx_stdout_priority, NULL))
		return (1);
	i = 0;
	while (i < sim_data->run_opts->num_philos)
	{
		if (pthread_mutex_init(sim_data->mtx_forks + i, NULL))
			return (1);
		++i;
	}

	return (0);
}

int				main(int argc, char **argv)
{
	int				i;
	t_sim_data		sim_data;

	if (!(sim_data.run_opts = parse_args(argc, argv)) || \
		init_sim_data(&sim_data) || \
		start_threads(&sim_data))
		return (1);
	i = 0;
	while (i < sim_data.run_opts->num_philos)
		pthread_join((sim_data.threads)[i++], NULL);

	// Do cleanup.
	return (0);
}
