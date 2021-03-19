/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 08:04:46 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/23 12:34:14 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

uint64_t		get_ms_time(void)
{
	struct timeval		cur;

	gettimeofday(&cur, NULL);
	return ((uint64_t)cur.tv_sec * 1000 + cur.tv_usec / 1000);
}

int				philo_is_dead(t_philo_status *philo_status)
{
	t_sim_data		*sim_data;

	sim_data = (t_sim_data *)philo_status->sim_data;
	if (get_ms_time() - philo_status->last_time_eaten >= sim_data->run_opts->time_to_die)
	{
		pthread_mutex_lock(&sim_data->mtx_stdout_priority);
		if (sim_data->sim_is_running)
		{
			sim_data->sim_is_running = 0;
			pthread_mutex_lock(&sim_data->mtx_stdout_normal);
			printf("%-8llu %d died\n", get_ms_time() - sim_data->sim_start, philo_status->id);
			pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		}
		pthread_mutex_unlock(&sim_data->mtx_stdout_priority);
		return (1);
	}
	return (0);
}

void			*philo_start(void *arg)
{
	t_philo_status	*philo_status;
	t_sim_data		*sim_data;
	int64_t			time_to_live;
	int				state_changed;

	philo_status = (t_philo_status *)arg;
	sim_data = (t_sim_data *)philo_status->sim_data;
	state_changed = 1;
	while (1)
	{
		if (state_changed)
		{
			pthread_mutex_lock(&sim_data->mtx_stdout_normal);
			if (sim_data->sim_is_running)
				printf("%-8llu %d is thinking\n", get_ms_time() - sim_data->sim_start, philo_status->id);
			pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
			state_changed = 0;
		}
		time_to_live = sim_data->run_opts->time_to_die - get_ms_time() + philo_status->last_time_eaten;
		if (time_to_live <= 0)
		{
			philo_is_dead(philo_status);
			return (NULL);
		}
		usleep(time_to_live * 500);  // the less is 'time_to_live' - the shorter the wait

		// Even philos take the right fork first, odd philos - the left one.
		if (pthread_mutex_lock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
			philo_status->left_fork_idx : philo_status->right_fork_idx]))
			continue ;
		if (!sim_data->sim_is_running || philo_is_dead(philo_status))
		{
			pthread_mutex_unlock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
				philo_status->left_fork_idx : philo_status->right_fork_idx]);
			return (NULL);
		}
		pthread_mutex_lock(&sim_data->mtx_stdout_normal);
		if (sim_data->sim_is_running)
			printf("%-8llu %d has taken a fork\n", get_ms_time() - sim_data->sim_start, philo_status->id);
		pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		state_changed = 1;

		if (pthread_mutex_lock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
			philo_status->right_fork_idx : philo_status->left_fork_idx]))
		{
			pthread_mutex_unlock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
				philo_status->left_fork_idx : philo_status->right_fork_idx]);
			continue ;
		}
		if (!sim_data->sim_is_running || philo_is_dead(philo_status))
		{
			pthread_mutex_unlock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
				philo_status->right_fork_idx : philo_status->left_fork_idx]);
			pthread_mutex_unlock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
			philo_status->left_fork_idx : philo_status->right_fork_idx]);
			return (NULL);
		}
		pthread_mutex_lock(&sim_data->mtx_stdout_normal);
		if (sim_data->sim_is_running)
			printf("%-8llu %d has taken a fork\n", get_ms_time() - sim_data->sim_start, philo_status->id);
		if (sim_data->sim_is_running)
			printf("%-8llu %d is eating\n", get_ms_time() - sim_data->sim_start, philo_status->id);
		pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		usleep(sim_data->run_opts->time_to_eat * 1000);
		pthread_mutex_unlock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
				philo_status->right_fork_idx : philo_status->left_fork_idx]);
		pthread_mutex_unlock(&sim_data->mtx_forks[(philo_status->id % 2) ? \
			philo_status->left_fork_idx : philo_status->right_fork_idx]);
		philo_status->last_time_eaten = get_ms_time();
		++philo_status->cur_eat_cycles;
		if (philo_status->cur_eat_cycles == sim_data->run_opts->num_eat_cycles)
			--sim_data->unfinished_philos;
		if (sim_data->unfinished_philos == 0)
		{
			sim_data->sim_is_running = 0;
			return (NULL);
		}
		if (!sim_data->sim_is_running)
			return (NULL);
		pthread_mutex_lock(&sim_data->mtx_stdout_normal);
		if (sim_data->sim_is_running)
			printf("%-8llu %d is sleeping\n", get_ms_time() - sim_data->sim_start, philo_status->id);
		pthread_mutex_unlock(&sim_data->mtx_stdout_normal);
		usleep(sim_data->run_opts->time_to_sleep * 1000);
		if (!sim_data->sim_is_running || philo_is_dead(philo_status))
			return (NULL);
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

	sim_data->sim_start = get_ms_time();
	sim_data->sim_is_running = 1;
	sim_data->unfinished_philos = sim_data->run_opts->num_philos;

	// Init philo_stat_tab.
	i = 0;
	while (i < sim_data->run_opts->num_philos)
	{
		(sim_data->philo_stat_tab)[i].id = i + 1;
		(sim_data->philo_stat_tab)[i].left_fork_idx = i;
		(sim_data->philo_stat_tab)[i].right_fork_idx = (i == 0) ? sim_data->run_opts->num_philos - 1 : i - 1;
		(sim_data->philo_stat_tab)[i].cur_eat_cycles = 0;
		(sim_data->philo_stat_tab)[i].last_time_eaten = sim_data->sim_start;
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
		pthread_join(sim_data.threads[i++], NULL);

	// Do cleanup if necessery.
	return (0);
}
