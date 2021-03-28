/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 08:04:46 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 20:03:59 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static int		int_start_proc(t_sim_data *sim_data, int id)
{
	sim_data->philo_status.id = id;
	if ((sim_data->philos[id - 1] = fork()) < 0)
		return (1);
	else if (sim_data->philos[id - 1] == 0)
	{
		pthread_create(&sim_data->monitor, NULL, monitor_start, \
			&sim_data->philo_status);
		philo_start(&sim_data->philo_status);
	}
	return (0);
}

int				start_proc(t_sim_data *sim_data)
{
	int			i;

	sim_data->sim_start = get_microsec();
	sim_data->philo_status.last_time_eaten = sim_data->sim_start;
	sim_data->philo_status.cur_eat_cycles = 0;
	sim_data->philo_status.sim_data = sim_data;
	i = 0;
	while (i < sim_data->num_philos)
	{
		if (int_start_proc(sim_data, i + 1))
			return (1);
		i += 2;
	}
	microsleep(10);
	i = 1;
	while (i < sim_data->num_philos)
	{
		if (int_start_proc(sim_data, i + 1))
			return (1);
		i += 2;
	}
	return (0);
}

int				main(int argc, char **argv)
{
	int				i;
	t_sim_data		sim_data;

	if (parse_args(argc, argv, &sim_data) || \
		init_sim_data(&sim_data) || start_proc(&sim_data))
		return (1);
	i = 0;
	while (i++ < sim_data.num_philos)
		sem_wait(sim_data.sem_unfinished_philos);
	i = 0;
	while (i < sim_data.num_philos)
		kill(sim_data.philos[i++], 9);
	clear_sim_data(&sim_data);
	return (0);
}
