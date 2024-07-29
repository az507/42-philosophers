/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 14:18:01 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*	philo_utils.c	*/
void	philo_setup(t_params *params);
void	ft_usleep(t_params *params, int usleep_amt);

static void	philo_eat(t_params *params)
{
	if (sem_wait(params->sem_forks) == -1)
		sem_perror(params, "sem_wait-philo_eat");
	sem_printf(params, TAKE_FORK);
	sem_printf(params, EAT);
	ft_usleep(params, params->info.time_eat);
	if (sem_post(params->sem_forks) == -1)
		sem_perror(params, "sem_post-philo_eat");
	params->time_last_ate = get_time_ms(params);
}

static void	philo_sleep(t_params *params)
{
	sem_printf(params, SLEEP);
	ft_usleep(params, params->info.time_sleep);
}

static void	philo_think(t_params *params)
{
	sem_printf(params, THINK);
}

static void	philo_track(t_params *params)
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

void	philo_routine(t_params *params)
{
	philo_setup(params);
	while (true)
	{
		philo_eat(params);
		if (params->info.track_philo_quotas)
			philo_track(params);
		philo_sleep(params);
		philo_think(params);
	}
}
