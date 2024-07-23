/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:01:46 by achak             #+#    #+#             */
/*   Updated: 2024/04/29 15:59:45 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define SECS_PER_DAY 86400
# define MILISECS_PER_SEC 1000
# define MICROSECS_PER_MS 1000

# define DEFAULT 0
# define ONE_PHILO_DEAD 1
# define ALL_PHILOS_EATEN 2
# define TERMINATE 3

# define ERROR 1

typedef struct s_shared
{
	int				philo_max;
	long			time_die;
	long			time_eat;
	long			time_sleep;
	long			start_time;
	int				eat_max;
	bool			start_flag;
	int				loop_flag;
	int				full_philos_nbr;
	int				dead_philo_nbr;
	long			time_of_death;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	shared_mutex;
	pthread_mutex_t	full_philos_mutex;
}	t_shared;

typedef struct s_philo
{
	int				philo_nbr;
	int				eat_count;
	long			time_last_ate;
	bool			done_eating;
	pthread_mutex_t	local_mutex;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	t_shared		*shared_args;
}	t_philo;

//	main.c
void	create_and_join_threads(t_philo *philo_arr, t_shared *shared_args);

//	philo_routine.c
void	wait_sync_start(t_shared *shared_args);
void	*one_philo_routine(t_philo *routine, t_shared *shared_args);
void	*philo_routine(void *arg);

//	philo_actions.c
int		printf_with_mutex(t_philo *philo, t_shared *shared_args,
			const char *msg);
bool	is_philo_done_eating(t_philo *philo, t_shared *shared_args);
int		eat(t_philo *philo, t_shared *shared_args);
int		philo_sleep(t_philo *philo, t_shared *shared_args);
int		think(t_philo *philo, t_shared *shared_args);

//	philo_actions2.c
int		pick_up_forks(t_philo *philo, t_shared *shared_args);
void	release_forks(t_philo *philo);

//	track_philos_routine.c
void	init_philos_time_last_ate(t_philo **philo_arr);
int		find_dead_philo(t_philo **philo_arr, t_shared *shared_args);
void	track_dead_and_full_philos(t_philo **philo_arr,
			t_shared *shared_args);
void	*track_routine(void *arg);

//	init_args.c
int		validate_args(int ac, char **av);
void	init_shared_args(t_shared *shared_args, int ac, char **av);
void	assign_forks_to_philo(t_philo *philo_arr, int i, int philo_max,
			pthread_mutex_t *fork_arr);
t_philo	*init_philo_arr(t_shared *shared_args);

//	helper_fns.c
void	pwrap(int func_rv, int func_type);
int		ft_atoi(const char *nptr);
long	get_time_ms(void);
void	perform_cleanup(t_philo *philo_arr, t_shared *shared_args);

#endif
