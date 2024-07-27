/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:04:24 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 17:31:33 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <signal.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <stdbool.h>

# define MS_TO_MICROSEC 1000
# define SECS_TO_MS 1000
# define SEM_FORKS "forks"
# define SEM_LOCK "lock"
# define SEM_PRINT "print"
# define TAKE_FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DEAD "died"
# define TRACK_PHILOS_EAT true
# define PHILO 0
# define MAIN 1

typedef struct s_info
{
	int	philo_max;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	eat_max;
}	t_info;

typedef struct s_addon
{
	pthread_t (*setup_fptr)(struct *s_addon, int ptype);
	void	(*philo_track)(struct *s_addon);
	sem_t	*sem_count;
	sem_t	*sem_plock;
	int	times_ate;
}	t_addon;

typedef struct s_params
{
	t_info	info;
	pid_t	*pids;
	int	philo_id;
	int	time_last_ate;
	struct timeval	start_tv;
	sem_t	*sem_forks;
	sem_t	*sem_print;
	t_addon	*addon;
}	t_params;

typedef struct s_helper
{
	t_params	*philo;
	sem_t	*sem_tlock;
	bool	term_cond;
}	t_helper;

void	sem_printf(t_params *params, const char *str);

void	philo_destroy(t_params *params);

void	ft_error(t_params *params, const char *error_msg);

void	ft_putendl_fd(const char *msg, int fd);
void	*ft_malloc(size_t size);
long	get_time_ms(long start_time);

#endif
