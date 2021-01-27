/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 11:20:20 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/19 02:42:55 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct		s_run_opts
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat_cycles;
}					t_run_opts;

typedef struct		s_philo_status
{
	int				id;
	int				cur_eat_cycles;
	struct timeval	tv_last_time_eaten;
	void			*sim_data;
}					t_philo_status;

typedef struct		s_sim_data
{
	struct timeval	tv_sim_start;
	int				sim_is_running;
	t_run_opts		*run_opts;
	t_philo_status	*philo_stat_tab;
	pthread_t		*threads;
	pthread_mutex_t	*mtx_forks;
	pthread_mutex_t	mtx_stdout_normal;
	pthread_mutex_t	mtx_stdout_priority;
}					t_sim_data;

char		*ft_strchr(const char *str, int c);
int			ft_atoi(const char *str);
int			check_args(int argc, char **argv);
t_run_opts	*parse_args(int argc, char **argv);

#endif
