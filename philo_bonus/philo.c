/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/25 17:02:11 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	philo_eat(t_params *params)
{
	if (sem_wait(params->sem_forks) == -1)
		ft_putendl_fd("sem_wait() in philo_eat", STDERR_FILENO);
	printf("%ld philosopher %d has taken a fork\n",
		get_time_ms(params->start_time), params->philo_id);
	if (sem_wait(params->sem_forks) == -1)
		ft_putendl_fd("sem_wait() in philo_eat", STDERR_FILENO);
	printf("%ld philosopher %d has taken a fork\n",
		get_time_ms(params->start_time), params->philo_id);
	printf("%ld philosopher %d is eating\n",
		get_time_ms(params->start_time), params->philo_id);
	if (usleep(params->time_eat) == -1)
		ft_putendl_fd("usleep() in philo_eat", STDERR_FILENO);
	if (sem_post(params->sem_forks) == -1)
		ft_putendl_fd("sem_post() in philo_eat", STDERR_FILENO);
	if (sem_post(params->sem_forks) == -1)
		ft_putendl_fd("sem_post() in philo_eat", STDERR_FILENO);
}

static void	philo_sleep(t_params *params)
{
	printf("%ld philosopher %d is sleeping\n",
		get_time_ms(params->start_time), params->philo_id);
	if (usleep(params->time_sleep) == -1)
		ft_putendl_fd("usleep() in philo_sleep", STDERR_FILENO);
}

static void	philo_think(t_params *params)
{
	printf("%ld philosopher %d is thinking\n",
		get_time_ms(params->start_time), params->philo_id);
}

void	philo_routine(t_params *params)
{
	struct timeval	tv;
	int	times_ate;

	params->sem_forks = sem_open(SEM_FORKS, O_WRONLY);
	if (params->sem_forks == SEM_FAILED)
		ft_putendl_fd("sem_open() in philo routine", STDERR_FILENO);
	times_ate = 0;
	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday() in philo_routine", STDERR_FILENO);
	params->start_time = tv.tv_sec;
	if (params->philo_id % 2 == 0)
		philo_sleep(params);
	while (true)
	{
		philo_eat(params);
		if (++times_ate == params->eat_max)
		{
			int res = kill(params->pids[0], SIGSTOP);
			printf("=========================================================================        res = %d\n", res);
		}
		philo_sleep(params);
		philo_think(params);
	}
}
