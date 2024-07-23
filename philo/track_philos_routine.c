/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   track_philos_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:57:56 by achak             #+#    #+#             */
/*   Updated: 2024/04/29 11:13:25 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philos_time_last_ate(t_philo **philo_arr)
{
	int	i;

	i = -1;
	while (++i < (*philo_arr)->shared_args->philo_max)
	{
		pwrap(pthread_mutex_lock(&((*philo_arr)[i]).local_mutex), 6);
		if ((*philo_arr)[i].time_last_ate == 0)
			(*philo_arr)[i].time_last_ate = get_time_ms();
		pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
	}
}

int	find_dead_philo(t_philo **philo_arr, t_shared *shared_args)
{
	int	i;
	int	flag;

	i = -1;
	flag = 0;
	while (++i < shared_args->philo_max)
	{
		pwrap(pthread_mutex_lock(&((*philo_arr)[i]).local_mutex), 6);
		if (get_time_ms() > (*philo_arr)[i].time_last_ate
			+ shared_args->time_die
			&& ((*philo_arr)[i]).done_eating == false)
			flag = 1;
		pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
		if (flag == 1)
		{
			shared_args->dead_philo_nbr = i + 1;
			shared_args->time_of_death
				= get_time_ms() - shared_args->start_time;
			pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
			shared_args->loop_flag = ONE_PHILO_DEAD;
			pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
			return (1);
		}
	}
	return (0);
}

void	track_dead_and_full_philos(t_philo **philo_arr, t_shared *shared_args)
{
	int	flag;

	flag = 0;
	while (1)
	{
		pwrap(pthread_mutex_lock(&shared_args->full_philos_mutex), 6);
		if (shared_args->full_philos_nbr == shared_args->philo_max)
			flag = 2;
		pwrap(pthread_mutex_unlock(&shared_args->full_philos_mutex), 7);
		if (flag == 2)
		{
			pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
			printf("%ld all philosophers are done eating\n",
				get_time_ms() - shared_args->start_time);
			pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
			return ;
		}
		if (find_dead_philo(philo_arr, shared_args) == 1)
			return ;
	}
}

void	*track_routine(void *arg)
{
	t_philo		**philo_arr;
	t_shared	*shared_args;

	philo_arr = (t_philo **)arg;
	shared_args = (*philo_arr)->shared_args;
	wait_sync_start((*philo_arr)->shared_args);
	if (shared_args->philo_max == 1)
		return (NULL);
	init_philos_time_last_ate(philo_arr);
	if (shared_args->eat_max == -1)
		while (find_dead_philo(philo_arr, shared_args) != 1)
			;
	else
		track_dead_and_full_philos(philo_arr, shared_args);
	return (NULL);
}

/*
		//track_dead_philo_only(philo_arr, shared_args);
//	while (1)
//	{
//		i = -1;
//		count = 0;
//		while (++i < shared_args->philo_max)
//		{
//			pwrap(pthread_mutex_lock(&((*philo_arr)[i]).local_mutex), 6);
//			if ((*philo_arr)[i].time_last_ate == 0)
//			{
//				pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
//				continue ;
//			}
//			if ((*philo_arr)[i].eat_count >= shared_args->eat_max
//				&& shared_args->eat_max != -1)
//				count++;
//			else if (get_time_ms() > (*philo_arr)[i].time_last_ate
//				+ shared_args->time_die)
//				temp_flag = 1;
//			pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
//			if (temp_flag)
//			{
//				pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
//				shared_args->dead_philo_nbr = i + 1;
//				shared_args->time_of_death = get_time_ms()
//					- shared_args->start_time;
//				shared_args->loop_flag = ONE_PHILO_DEAD;
//				pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
//				return (NULL);
//			}
//		}
//		//usleep(20000);
////		pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
////		printf("----------->>>>count = %d\n", count);
////		pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
//		if (count == shared_args->philo_max && shared_args->eat_max != -1)
//		{
//			pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
//			shared_args->loop_flag = ALL_PHILOS_EATEN;
//			printf("\n\n\n\nALL PHILOS ARE DONE EATING!!!\n\n\n\n");
//			pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
//			return (NULL);
//		}
//	}
void	track_dead_philo_only(t_philo **philo_arr, t_shared *shared_args)
{
	int	i;
	int	flag;

	flag = 0;
	while (1)
	{
		if (find_dead_philo(philo_arr, shared_args) == 1)
			return ;
//		i = -1;
//		while (++i < shared_args->philo_max)
//		{
//			pwrap(pthread_mutex_lock(&((*philo_arr)[i]).local_mutex), 6);
//			if (get_time_ms() > (*philo_arr)[i].time_last_ate
//				+ shared_args->time_die)
//				flag = 1;
//			pwrap(pthread_mutex_unlock(&((*philo_arr)[i]).local_mutex), 7);
//			if (flag)
//			{
//				shared_args->dead_philo_nbr = i + 1;
//				shared_args->time_of_death =
//					get_time_ms() - shared_args->start_time;
//				pwrap(pthread_mutex_lock(&shared_args->shared_mutex), 6);
//				shared_args->loop_flag = ONE_PHILO_DEAD;
//				pwrap(pthread_mutex_unlock(&shared_args->shared_mutex), 7);
//				return ;
//			}
//		}
	}
}

*/
