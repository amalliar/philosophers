/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 11:20:20 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/24 17:05:43 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>
# include <stddef.h>
# include <limits.h>
# include <stdint.h>
# include <sys/time.h>

typedef struct		s_philo_status
{
	int				id;
	int				cur_eat_cycles;
	uint64_t		last_time_eaten;
	void			*sim_data;
}					t_philo_status;

typedef struct		s_sim_data
{
	uint64_t		sim_start;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat_cycles;

	

	sem_t			*sem_forks;
	sem_t			*sem_stdout;



	int				unfinished_philos;
	t_philo_status	*philo_stat_tab;
	pthread_t		*philos;
	pthread_t		monitor;
}					t_sim_data;

char				*ft_strchr(const char *str, int c);
int					ft_atoi(const char *str);
int					init_sim_data(t_sim_data *sim_data);
int					parse_args(int argc, char **argv, t_sim_data *sim_data);
uint64_t			get_timestamp(void);
void				*monitor_start(void *arg);
void				*philo_start(void *arg);
void				clear_sim_data(t_sim_data *sim_data);
void				philo_take_forks(t_philo_status *philo_status);
void				philo_release_forks(t_philo_status *philo_status);
void				print_status(t_philo_status *philo_status, const char *msg);

#endif
