/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 18:43:04 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "params_bonus.h"

void	sem_printf(t_params *params, const char *str)
{
	sem_wait(params->sem_print);
	printf("%ld paramssopher %d %s\n", get_time_ms(params->start_time),
		params->params_id, str);
	sem_post(params->sem_print);
}

void	sem_perror(t_params *params, const char *error)
{
	sem_wait(params->sem_print);
	ft_putendl_fd(error, STDERR_FILENO);
	sem_post(params->sem_print);
}

static void	philo_eat(t_params *params)
{
	if (sem_wait(sem_forks) == -1)
		sem_perror(params, "sem_wait-philo_eat");
	sem_printf(params, TAKE_FORK);
	sem_printf(params, TAKE_FORK);
	sem_printf(params, EAT);
	if (usleep(params->time_eat) == -1)
		sem_perror(params, "usleep-params_eat");
	if (sem_post(sem_forks) == -1)
		sem_perror(params, "sem_post-philo_eat");
}

static void	philo_sleep(t_params *params)
{
	sem_printf(params, SLEEP);
	if (usleep(params->time_sleep) == -1)
		ft_putendl_fd("usleep-params_sleep", STDERR_FILENO);
}

static void	philo_think(t_params *params)
{
	sem_printf(params, THINK);
}

void	philo_track(t_params *params)
{
	static int	times_ate = 0;

	if (++times_ate == params->info.eat_max)
	{
		if (sem_wait(params->sem_plock) == -1)
			sem_perror(params, "sem_post-philo_track");
		if (sem_post(params->sem_count) == -1)
			sem_perror(params, "sem_wait-philo_track");
	}
}

// sigtrap here so that i can reliably use sem_print for sem_perror function calls
void	philo_setup(t_params *params)
{
	params->sem_forks = sem_open(SEM_FORKS, 0);
	params->sem_print = sem_open(SEM_PRINT, 0);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (ft_putendl_fd("sem_open-philo_setup", STDERR_FILENO),
			(void)kill(0, SIGTRAP));
	if (params->track_philos_quota)
	{
		philo->sem_count = sem_open(SEM_COUNT, 0);
		philo->sem_plock = sem_open(SEM_PLOCK, 0);
		if (params->sem_count == SEM_FAILED || params->plock == SEM_FAILED)
			sem_perror(params, "sem_open-philo_setup");
	}
	if (gettimeofday(&params->start_tv, NULL) == -1)
		sem_perror(params, "gettimeofday-philo_setup");
	free(params->pids);
	params->pids = NULL;
}

void	philo_routine(t_params *params)
{
	philo_setup(params);
	while (true)
	{
		philo_eat(params);
		if (params->info.track_philos_quota)
			philo_track(params);
		philo_sleep(params);
		philo_think(params);
	}
}
