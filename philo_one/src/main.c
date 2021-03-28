/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 08:04:46 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 12:14:57 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int				start_threads(t_sim_data *sim_data)
{
	int			i;

	sim_data->sim_start = get_microsec();
	i = 0;
	while (i < sim_data->num_philos)
	{
		sim_data->philo_stat_tab[i].last_time_eaten = sim_data->sim_start;
		if (pthread_create(&sim_data->philos[i], NULL, philo_start, \
			&sim_data->philo_stat_tab[i]))
			return (1);
		i += 2;
	}
	i = 1;
	while (i < sim_data->num_philos)
	{
		sim_data->philo_stat_tab[i].last_time_eaten = sim_data->sim_start;
		if (pthread_create(&sim_data->philos[i], NULL, philo_start, \
			&sim_data->philo_stat_tab[i]))
			return (1);
		i += 2;
	}
	if (pthread_create(&sim_data->monitor, NULL, monitor_start, sim_data) || \
		pthread_detach(sim_data->monitor))
		return (1);
	return (0);
}

int				main(int argc, char **argv)
{
	int				i;
	t_sim_data		sim_data;

	if (parse_args(argc, argv, &sim_data) || \
		init_sim_data(&sim_data) || start_threads(&sim_data))
		return (1);
	i = 0;
	while (i < sim_data.num_philos)
		pthread_join(sim_data.philos[i++], NULL);
	clear_sim_data(&sim_data);
	return (0);
}
