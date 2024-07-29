/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 19:19:20 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*	philo_utils.c	*/
void	philo_setup(t_params *params);
void	ft_usleep(t_params *params, int usleep_amt);

static void	philo_eat(t_params *params)
{
	sem_wait(params->sem_forks);
	sem_printf(params, TAKE_FORK);
	sem_printf(params, EAT);
	ft_usleep(params, params->info.time_eat);
	sem_post(params->sem_forks);
	params->time_last_ate = get_time_ms(params);
}

static void	philo_sleep(t_params *params)
{
	sem_printf(params, SLEEP);
	ft_usleep(params, params->info.time_sleep);
}

static void	philo_think(t_params *params)
{
	usleep(1000);
	sem_printf(params, THINK);
}

void	philo_routine(t_params *params)
{
	int	times_ate;

	times_ate = 0;
	philo_setup(params);
	while (true)
	{
		philo_eat(params);
		if (params->info.track_philo_quotas
			&& ++times_ate == params->info.philo_max)
			sem_post(params->sem_count);
		philo_sleep(params);
		philo_think(params);
	}
}
