/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:34:01 by achak             #+#    #+#             */
/*   Updated: 2024/04/29 16:11:39 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	printf_with_mutex(t_philo *philo, t_shared *shared_args, const char *msg)
{
	pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
	if (shared_args->loop_flag == DEFAULT)
		printf("%ld philosopher %d %s\n",
			get_time_ms() - shared_args->start_time,
			philo->philo_nbr, msg);
	else if (shared_args->loop_flag == ONE_PHILO_DEAD)
	{
		printf("%ld philosopher %d died\n",
			shared_args->time_of_death,
			shared_args->dead_philo_nbr);
		shared_args->loop_flag = TERMINATE;
		pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
		return (TERMINATE);
	}
	else
	{
		pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
		return (TERMINATE);
	}
	pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
	return (DEFAULT);
}

bool	is_philo_done_eating(t_philo *philo, t_shared *shared_args)
{
	if (shared_args->eat_max != -1)
	{
		philo->eat_count++;
		if (philo->eat_count == shared_args->eat_max)
		{
			pwrap(pthread_mutex_lock(&shared_args->full_philos_mutex), 6);
			shared_args->full_philos_nbr++;
			pwrap(pthread_mutex_unlock(&shared_args->full_philos_mutex), 7);
			pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
			philo->done_eating = true;
			pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
			return (true);
		}
	}
	return (false);
}

int	eat(t_philo *philo, t_shared *shared_args)
{
	pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
	philo->time_last_ate = get_time_ms();
	pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
	if (printf_with_mutex(philo, shared_args, "is eating") == TERMINATE)
	{
		release_forks(philo);
		return (TERMINATE);
	}
	usleep(shared_args->time_eat * MICROSECS_PER_MS);
	release_forks(philo);
	if (is_philo_done_eating(philo, shared_args) == true)
		return (TERMINATE);
	return (DEFAULT);
}

int	philo_sleep(t_philo *philo, t_shared *shared_args)
{
	if (printf_with_mutex(philo, shared_args, "is sleeping") == TERMINATE)
		return (TERMINATE);
	usleep(shared_args->time_sleep * MICROSECS_PER_MS);
	return (DEFAULT);
}

int	think(t_philo *philo, t_shared *shared_args)
{
	if (printf_with_mutex(philo, shared_args, "is thinking") == TERMINATE)
		return (TERMINATE);
	usleep(1000);
	return (DEFAULT);
}
