/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:04:24 by achak             #+#    #+#             */
/*   Updated: 2024/07/31 20:16:16 by achak            ###   ########.fr       */
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
# define SEM_FORKS "/forks"
# define SEM_PRINT "/print"
# define SEM_COUNT "/count"
# define TAKE_FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"

typedef struct s_info
{
	int		philo_max;
	int		time_die;
	int		time_eat;
	int		time_sleep;
	int		eat_max;
	bool	track_philos_quota;
}	t_info;

typedef struct s_params
{
	struct timeval	start_tv;
	t_info			info;
	pid_t			*pids;
	sem_t			*sem_forks;
	sem_t			*sem_print;
	sem_t			*sem_count;
	long			time_last_ate;
	int				philo_id;
}	t_params;

void	processes_cleanup(t_params *params);
void	params_destroy(t_params *params);
void	ft_error(t_params *params, const char *error_msg);

void	ft_sem_open(t_params *params, int oflag, mode_t mode);
void	ft_putendl_fd(const char *msg, int fd);
long	get_time_ms(t_params *params);

#endif
