/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:04:24 by achak             #+#    #+#             */
/*   Updated: 2024/07/24 18:08:17 by achak            ###   ########.fr       */
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
# define SEM_NAME "forks"

typedef struct s_params
{
	int		philo_max;
	int		time_die;
	int		time_eat;
	int		time_sleep;
	int		eat_max;
	int		philo_id;
	int		time_last_ate;
	pid_t	*pids;
	sem_t	*sem_forks;
	//t_philo	*philo_arr;
}	t_params;

void	params_destroy(t_params *params);

void	ft_error(t_params *params, const char *error_msg);

void	ft_putendl_fd(const char *msg, int fd);
void	*ft_malloc(size_t size);

#endif
