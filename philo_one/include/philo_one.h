/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalliar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/23 11:20:20 by amalliar          #+#    #+#             */
/*   Updated: 2021/03/28 12:45:33 by amalliar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stddef.h>
# include <limits.h>
# include <stdint.h>
# include <sys/time.h>

enum				e_fork_states
{
	TAKEN,
	AVAILABLE
};

typedef struct		s_philo_status
{
	int				id;
	int				left_fork_idx;
	int				right_fork_idx;
	int				cur_eat_cycles;
	uint64_t		last_time_eaten;
	void			*sim_data;
}					t_philo_status;

typedef struct		s_sim_data
{
	uint64_t		sim_start;
	int				sim_is_running;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	int				num_eat_cycles;
	int				unfinished_philos;
	int				*forks;
	t_philo_status	*philo_stat_tab;
	pthread_t		*philos;
	pthread_t		monitor;
	pthread_mutex_t	mtx_forks;
	pthread_mutex_t	mtx_stdout;
}					t_sim_data;

char				*ft_strchr(const char *str, int c);
int					ft_atoi(const char *str);
int					init_sim_data(t_sim_data *sim_data);
int					parse_args(int argc, char **argv, t_sim_data *sim_data);
uint64_t			get_microsec(void);
void				*monitor_start(void *arg);
void				*philo_start(void *arg);
void				clear_sim_data(t_sim_data *sim_data);
void				philo_take_forks(t_philo_status *philo_status);
void				philo_release_forks(t_philo_status *philo_status);
void				print_status(t_philo_status *philo_status, const char *msg);
void				microsleep(uint64_t microsec);

#endif
