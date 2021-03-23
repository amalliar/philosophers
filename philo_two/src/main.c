/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 08:04:46 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/23 18:56:18 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int				start_threads(t_sim_data *sim_data)
{
	int		i;

	i = 0;
	while (i < sim_data->num_philos)
	{
		if (pthread_create(&sim_data->threads[i], NULL, philo_start, \
			&sim_data->philo_stat_tab[i]))
			return (1);
		++i;
	}
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
		pthread_join(sim_data.threads[i++], NULL);
	clear_sim_data(&sim_data);
	return (0);
}
