/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/31 10:51:59 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	sem_printf(t_params *params, const char *str)
{
	long	current_time;

	sem_wait(params->sem_print);
	current_time = get_time_ms(params);
	printf("%ld philosopher %d %s\n", current_time, params->philo_id, str);
	if (str[0] == 'h')
		printf("%ld philosopher %d %s\n", current_time, params->philo_id, str);
	sem_post(params->sem_print);
}

void	ft_usleep(t_params *params, int usleep_amt)
{
	long	time_of_death;
	long	current_time;

	time_of_death = params->time_last_ate + params->info.time_die;
	current_time = get_time_ms(params);
	if (current_time + usleep_amt < time_of_death)
		usleep(usleep_amt * MS_TO_MICROSEC);
	else
	{
		usleep((time_of_death - current_time) * MS_TO_MICROSEC);
		params_destroy(params);
		exit(EXIT_SUCCESS);
	}
}

void	philo_eat(t_params *params)
{
	sem_wait(params->sem_forks);
	sem_printf(params, TAKE_FORK);
	sem_printf(params, EAT);
	ft_usleep(params, params->info.time_eat);
	sem_post(params->sem_forks);
	params->time_last_ate = get_time_ms(params);
}

void	philo_sleep(t_params *params)
{
	sem_printf(params, SLEEP);
	ft_usleep(params, params->info.time_sleep);
}

void	philo_think(t_params *params)
{
	usleep(1000);
	sem_printf(params, THINK);
}
