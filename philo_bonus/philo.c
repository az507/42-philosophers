/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 17:05:53 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sem_printf(t_philo *philo, const char *str)
{
	if (sem_wait(philo->sem_print) == -1)
		ft_putendl_fd("sem_wait-sem_printf");
	printf("%ld philosopher %d %s\n", get_time_ms(philo->start_time),
		philo->philo_id, str);
	if (sem_post(philo->sem_print) == -1)
		ft_putendl_fd("sem_post-sem_printf");
}

void	sem_perror(sem_t *sem_print, const char *error)
{
	sem_wait(sem_print);
	ft_putendl_fd(error, STDERR_FILENO);
	sem_post(sem_print);
}

static void	philo_eat(t_philo *philo, sem_t *sem_forks, sem_t *sem_print)
{
	if (sem_wait(sem_forks) == -1)
		sem_perror(philo, "sem_wait-philo_eat");
	sem_printf(philo, TAKE_FORK);
	sem_printf(philo, TAKE_FORK);
	sem_printf(philo, EAT);
	if (usleep(philo->time_eat) == -1)
		ft_putendl_fd("usleep-philo_eat", STDERR_FILENO);
	if (sem_post(sem_forks) == -1)
		ft_putendl_fd("sem_post-philo_eat", STDERR_FILENO);
}

static void	philo_sleep(t_philo *philo, sem_t *sem_print)
{
	sem_printf(philo, SLEEP);
	if (usleep(philo->time_sleep) == -1)
		ft_putendl_fd("usleep-philo_sleep", STDERR_FILENO);
}

static void	philo_think(t_philo *philo)
{
	sem_printf(philo, THINK);
}

void	philo_track(t_philo *philo)
{
	if (++(philo->times_ate) == philo->info.eat_max)
	{
		if (sem_wait(philo->sem_plock) == -1)
			ft_putendl_fd("sem_wait-philo_track", STDERR_FILENO);
		if (sem_post(philo->sem_count) == -1)
			ft_putendl_fd("sem_post-philo_track", STDERR_FILENO);
	}
}

void	philo_setup(t_philo *philo, pid_t **pids)
{
	philo->sem_forks = sem_open(SEM_FORKS, 0);
	philo->sem_print = sem_open(SEM_PRINT, 0);
	if (philo->sem_forks == SEM_FAILED || philo->sem_print == SEM_FAILED)
		ft_putendl_fd("sem_open-philo_routine", STDERR_FILENO);
	if (gettimeofday(&philo->start_tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-philo_routine", STDERR_FILENO);
	if (philo->setup_fptr)
		(*philo->setup_fptr)(philo, PHILO);
	free(*pids);
	*pids = NULL;
}

void	philo_routine(t_philo *philo, pid_t *pids)
{
	philo_setup(philo, &pids);
//	if (philo->philo_id % 2 == 0)
//		philo_sleep(philo, sems[2]);
	while (true)
	{
		philo_eat(philo, sems[0], sems[2]);
		if (philo->tracking_fptr)
			(*philo->tracking_fptr)(philo);
		philo_sleep(philo, sems[2]);
		philo_think(philo, sems[2]);
	}
}
