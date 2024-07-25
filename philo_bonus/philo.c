/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/25 13:47:59 by achak            ###   ########.fr       */
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

	times_ate = 0;
	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday() error in philo_routine", STDERR_FILENO);
	params->start_time = tv.tv_sec;
	if (params->philo_id % 2 == 0)
		usleep(params->time_sleep);
		//philo_sleep(params);
	while (true)
	{
		philo_eat(params);
		if (params->track_times_eaten && ++times_ate == params->eat_max)
			kill(0, SIGCONT);
		philo_sleep(params);
		philo_think(params);
	}
}
