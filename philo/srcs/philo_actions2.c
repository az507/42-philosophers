/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:58:44 by achak             #+#    #+#             */
/*   Updated: 2024/04/29 15:54:14 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	pick_up_forks(t_philo *philo, t_shared *shared_args)
{
	pwrap(pthread_mutex_lock(philo->first_fork), 6);
	if (printf_with_mutex(philo, shared_args,
			"has taken a fork") == TERMINATE)
	{
		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
		return (TERMINATE);
	}
	pwrap(pthread_mutex_lock(philo->second_fork), 6);
	if (printf_with_mutex(philo, shared_args,
			"has taken a fork") == TERMINATE)
	{
		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
		pwrap(pthread_mutex_unlock(philo->second_fork), 7);
		return (TERMINATE);
	}
	return (DEFAULT);
}

void	release_forks(t_philo *philo)
{
	pwrap(pthread_mutex_unlock(philo->first_fork), 7);
	pwrap(pthread_mutex_unlock(philo->second_fork), 7);
}
/*
int	pick_up_forks_odd_philo(t_philo *philo, t_shared *shared_args)
{
	pwrap(pthread_mutex_lock(philo->first_fork), 6);
//	if (printf_with_mutex(philo, shared_args,
//			"has taken a fork") == TERMINATE)
//	{
//		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
//		return (TERMINATE);
//	}
	pwrap(pthread_mutex_lock(philo->second_fork), 6);
	if (printf_with_mutex(philo, shared_args,
			"has taken a fork") == TERMINATE)
	{
		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
		pwrap(pthread_mutex_unlock(philo->second_fork), 7);
		return (TERMINATE);
	}
	if (printf_with_mutex(philo, shared_args,
			"has taken a fork") == TERMINATE)
	{
		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
		pwrap(pthread_mutex_unlock(philo->second_fork), 7);
		return (TERMINATE);
	}
	return (DEFAULT);
}
//	if (philo->philo_nbr % 2 == 0)
//	{
//		pwrap(pthread_mutex_lock(philo->second_fork), 6);
//		if (printf_with_mutex(philo, shared_args,
//				"has taken a fork") == TERMINATE)
//		{
//			pwrap(pthread_mutex_unlock(philo->second_fork), 7);
//			return (TERMINATE);
//		}
//		pwrap(pthread_mutex_lock(philo->first_fork), 6);
//		if (printf_with_mutex(philo, shared_args,
//				"has taken a fork") == TERMINATE)
//		{
//			pwrap(pthread_mutex_unlock(philo->second_fork), 7);
//			pwrap(pthread_mutex_unlock(philo->first_fork), 7);
//			return (TERMINATE);
//		}
//		if (printf_with_mutex(philo, shared_args,
//				"has taken a fork") == TERMINATE)
//		{
//			pwrap(pthread_mutex_unlock(philo->second_fork), 7);
//			pwrap(pthread_mutex_unlock(philo->first_fork), 7);
//			return (TERMINATE);
//		}
//	}
//	else if (pick_up_forks_odd_philo(philo, shared_args) == TERMINATE)
//		return (TERMINATE);
//	if (philo->philo_nbr % 2 == 0)
//	{
//		pwrap(pthread_mutex_unlock(philo->second_fork), 7);
//		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
//	}
//	else
//	{
//		pwrap(pthread_mutex_unlock(philo->first_fork), 7);
//		pwrap(pthread_mutex_unlock(philo->second_fork), 7);
//	}
*/
