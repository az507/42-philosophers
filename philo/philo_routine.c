/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:38:27 by achak             #+#    #+#             */
/*   Updated: 2024/04/29 15:48:52 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_sync_start(t_shared *shared_args)
{
	while (1)
	{
		pwrap(pthread_mutex_lock(&shared_args->start_mutex), 6);
		if (shared_args->start_flag == true)
		{
			pwrap(pthread_mutex_unlock(&shared_args->start_mutex), 7);
			return ;
		}
		pwrap(pthread_mutex_unlock(&shared_args->start_mutex), 7);
	}
}

void	*one_philo_routine(t_philo *philo, t_shared *shared_args)
{
	pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
	philo->time_last_ate = get_time_ms();
	pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
	pwrap(pthread_mutex_lock(philo->first_fork), 6);
	printf_with_mutex(philo, shared_args, "has taken a fork");
	while (1)
	{
		if (philo->time_last_ate + shared_args->time_die
			< get_time_ms())
		{
			printf("%ld philosopher %d died\n",
				get_time_ms() - shared_args->start_time,
				philo->philo_nbr);
			pwrap(pthread_mutex_unlock(philo->first_fork), 7);
			break ;
		}
	}
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_shared	*shared_args;

	philo = (t_philo *)arg;
	shared_args = philo->shared_args;
	wait_sync_start(shared_args);
	if (shared_args->philo_max == 1)
		return (one_philo_routine(philo, shared_args));
	if (philo->philo_nbr % 2 == 0)
		philo_sleep(philo, shared_args);
	while (1)
	{
		if (pick_up_forks(philo, shared_args) == TERMINATE)
			break ;
		if (eat(philo, shared_args) == TERMINATE)
			break ;
		if (philo_sleep(philo, shared_args) == TERMINATE)
			break ;
		if (think(philo, shared_args) == TERMINATE)
			break ;
	}
	return (NULL);
}

/*
//	if (shared_args->philo_max % 2 == 0 && philo->philo_nbr % 2 == 0)
//		delay_even_philos_start(philo);
//	pwrap(pthread_mutex_lock(&philo->local_mutex), 6);
//	philo->time_last_ate = get_time_ms();
//	pwrap(pthread_mutex_unlock(&philo->local_mutex), 7);
//	if (shared_args->philo_max % 2 == 0 && philo->philo_nbr % 2 == 0)
//		usleep(10000);
	//if (shared_args->philo_max % 2 == 0 && philo->philo_nbr % 2 == 0)
void	delay_even_philos_start(t_philo *philo, t_shared *shared_args)
{
	int	neighbour;

	neighbour = -1;
	if (philo->philo_nbr % 2 == 0)
	{
		neighbour = (philo->philo_nbr / 2) - 1;
		while (1)
		{
			pwrap(pthread_mutex_lock(&shared_args->start_mutex), 6);
			if (shared_args->state_arr[neighbour] == true)
			{
				pwrap(pthread_mutex_unlock(&shared_args->start_mutex), 7);
				return ;
			}
			pwrap(pthread_mutex_unlock(&shared_args->start_mutex), 7);
		}
	}
}

void	delay_even_philos_start(t_philo *philo)
{
	int	count;

//	if (philo->philo_nbr % 2 == 0)
//	{
	while (1)
	{
			count = 0;
		pwrap(pthread_mutex_lock(philo->first_neighbour_mutex), 6);
		if (*(philo->first_neighbour_flag) == true)
		{
			count++;
//			pwrap(pthread_mutex_unlock(philo->first_neighbour_mutex), 7);
//			return ;
		}
//			if (*(philo->first_neighbour_flag) == true)
//				count++;
		pwrap(pthread_mutex_unlock(philo->first_neighbour_mutex), 7);
		pwrap(pthread_mutex_lock(philo->second_neighbour_mutex), 6);
		if (*(philo->second_neighbour_flag) == true)
		{
			count++;
//			pwrap(pthread_mutex_unlock(philo->second_neighbour_mutex), 7);
//			return ;
		}
////			if (*(philo->second_neighbour_flag) == true)
////				count++;
		pwrap(pthread_mutex_unlock(philo->second_neighbour_mutex), 7);
		if (count == 2)
			return ;
	}
	//}
}
*/
