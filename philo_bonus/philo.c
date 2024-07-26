/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/26 19:01:16 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	philo_eat(t_params *params, sem_t *sem_forks, sem_t *sem_print)
{
	if (sem_wait(sem_forks) == -1)
		ft_putendl_fd("sem_wait-philo_eat", STDERR_FILENO);
	if (sem_wait(sem_print) == -1)
		perror("sem_wait-philo_eat");
	printf("%ld philosopher %d has taken a fork\n",
		get_time_ms(params->start_time), params->philo_id);
	if (sem_post(sem_print) == -1)
		perror("sem_post-philo_eat");
	if (sem_wait(sem_forks) == -1)
		ft_putendl_fd("sem_wait-philo_eat", STDERR_FILENO);
	if (sem_wait(sem_print) == -1)
		perror("sem_wait-philo_eat");
	printf("%ld philosopher %d has taken a fork\n",
		get_time_ms(params->start_time), params->philo_id);
	printf("%ld philosopher %d is eating\n",
		get_time_ms(params->start_time), params->philo_id);
	if (sem_post(sem_print) == -1)
		perror("sem_post-philo_eat");
	if (usleep(params->time_eat) == -1)
		ft_putendl_fd("usleep-philo_eat", STDERR_FILENO);
	if (sem_post(sem_forks) == -1)
		ft_putendl_fd("sem_post-philo_eat", STDERR_FILENO);
	if (sem_post(sem_forks) == -1)
		ft_putendl_fd("sem_post-philo_eat", STDERR_FILENO);
}

static void	philo_sleep(t_params *params, sem_t *sem_print)
{
	if (sem_wait(sem_print) == -1)
		perror("sem_wait-philo_sleep");
	printf("%ld philosopher %d is sleeping\n",
		get_time_ms(params->start_time), params->philo_id);
	if (sem_post(sem_print) == -1)
		perror("sem_post-philo_sleep");
	if (usleep(params->time_sleep) == -1)
		ft_putendl_fd("usleep-philo_sleep", STDERR_FILENO);
}

static void	philo_think(t_params *params, sem_t *sem_print)
{
	if (sem_wait(sem_print) == -1)
		perror("sem_wait-philo_sleep");
	printf("%ld philosopher %d is thinking\n",
		get_time_ms(params->start_time), params->philo_id);
	if (sem_post(sem_print) == -1)
		perror("sem_post-philo_sleep");
}

void	philo_routine(t_params *params)
{
	int	times_ate;
	struct timeval	tv;
	sem_t	*sems[3];

	times_ate = 0;
	sems[0] = sem_open(SEM_FORKS, 0);
	sems[1] = sem_open(SEM_LOCK, 0);
	sems[2] = sem_open(SEM_PRINT, 0);
	if (sems[0] == SEM_FAILED || sems[1] == SEM_FAILED || sems[2] == SEM_FAILED)
		perror("sem_open-philo_routine");
		//ft_putendl_fd("sem_open-philo_routine", STDERR_FILENO);
	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-philo_routine", STDERR_FILENO);
	params->start_time = tv.tv_sec;
	if (params->philo_id % 2 == 0)
		philo_sleep(params, sems[2]);
	while (true)
	{
		philo_eat(params, sems[0], sems[2]);
		if (++times_ate == params->eat_max)
		{
//			sem_wait(params->sem_lock);
//			kill(params->pids[0], SIGSTOP);
			if (sem_post(sems[1]) == -1)
				perror("\tsem_post--philo_routine");
		}
		philo_sleep(params, sems[2]);
		philo_think(params, sems[2]);
	}
}
