/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/25 19:51:21 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	philo_eat(t_params *params)
{
	if (sem_wait(params->sem_forks) == -1)
		ft_putendl_fd("sem_wait-philo_eat", STDERR_FILENO);
	printf("%ld philosopher %d has taken a fork\n",
		get_time_ms(params->start_time), params->philo_id);
	if (sem_wait(params->sem_forks) == -1)
		ft_putendl_fd("sem_wait-philo_eat", STDERR_FILENO);
	printf("%ld philosopher %d has taken a fork\n",
		get_time_ms(params->start_time), params->philo_id);
	printf("%ld philosopher %d is eating\n",
		get_time_ms(params->start_time), params->philo_id);
	if (usleep(params->time_eat) == -1)
		ft_putendl_fd("usleep-philo_eat", STDERR_FILENO);
	if (sem_post(params->sem_forks) == -1)
		ft_putendl_fd("sem_post-philo_eat", STDERR_FILENO);
	if (sem_post(params->sem_forks) == -1)
		ft_putendl_fd("sem_post-philo_eat", STDERR_FILENO);
}

static void	philo_sleep(t_params *params)
{
	printf("%ld philosopher %d is sleeping\n",
		get_time_ms(params->start_time), params->philo_id);
	if (usleep(params->time_sleep) == -1)
		ft_putendl_fd("usleep-philo_sleep", STDERR_FILENO);
}

static void	philo_think(t_params *params)
{
	printf("%ld philosopher %d is thinking\n",
		get_time_ms(params->start_time), params->philo_id);
}

void	philo_routine(t_params *params)
{
	int	times_ate;
	struct timeval	tv;

	times_ate = 0;
	params->sem_forks = sem_open(SEM_FORKS, O_RDWR);
	params->sem_lock = sem_open(SEM_LOCK, O_RDWR);
	if (params->sem_forks == SEM_FAILED || params->sem_lock == SEM_FAILED)
		ft_putendl_fd("sem_open-philo_routine", STDERR_FILENO);
	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-philo_routine", STDERR_FILENO);
	params->start_time = tv.tv_sec;
	if (params->philo_id % 2 == 0)
		philo_sleep(params);
	while (true)
	{
		philo_eat(params);
		if (++times_ate == params->eat_max)
		{
//			sem_wait(params->sem_lock);
//			kill(params->pids[0], SIGSTOP);
			sem_post(params->sem_lock);
		}
		philo_sleep(params);
		philo_think(params);
	}
}
