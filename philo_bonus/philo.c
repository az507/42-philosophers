/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:06:07 by achak             #+#    #+#             */
/*   Updated: 2024/07/24 17:29:56 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	philo_eat(t_params *params)
{
	int	res;
	
	res = sem_wait(params->sem_forks);
	res = sem_wait(params->sem_forks);
	if (res == -1)
		ft_putendl_fd("sem_wait() in philo_eat function", STDERR_FILENO);
	printf("%d eating\n", params->philo_id);
	if (usleep(params->time_eat) == -1)
		ft_putendl_fd("usleep() in philo_eat function", STDERR_FILENO);
	res = sem_post(params->sem_forks);
	res = sem_post(params->sem_forks);
	if (res == -1)
		ft_putendl_fd("sem_post() in philo_eat function", STDERR_FILENO);
}

static void	philo_sleep(t_params *params)
{
	printf("%d sleeping\n", params->philo_id);
	if (usleep(params->time_sleep) == -1)
		ft_putendl_fd("usleep() in philo_sleep function", STDERR_FILENO);
}

static void	philo_think(void)
{

}

void	philo_routine(t_params *params)
{
	if (params->philo_id % 2 == 0)
		philo_sleep(params);
	while (true)
	{
		philo_eat(params);
		philo_sleep(params);
		philo_think();
	}
}
