/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 15:30:45 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitor_philos_status(t_params *params)
{
	int	wstatus;
	int	i;

	while (true)
	{
		i = -1;
		while (++i < params->info.philo_max)
		{
			sem_wait(params->sem_print);
			wstatus = 0;
			sem_post(params->sem_print);
			if (waitpid(params->pids[i], &wstatus, WNOHANG) <= 0)
				continue ;
			philos_cleanup(params);
			if (WIFEXITED(wstatus))
			{
				printf("%ld philosopher %d died\n",
					get_time_ms(params), i + 1);
				if (params->info.track_philo_quotas)
					sem_post(params->sem_count); //
			}
			else if (WIFSIGNALED(wstatus))
				printf("%ld all philos are done eating\n",
					get_time_ms(params));
			return ;
		}
	}
}

//static void	*helper_routine(void *arg)
void	*helper_routine(void *arg)
{
	t_params	*params;
	int			nbr_philos_done;

	params = (t_params *)arg;
	nbr_philos_done = 0;
//	while (1)
//	{
//		params->sem_count = sem_open(SEM_COUNT, 0);
//		if (params->sem_count == SEM_FAILED)
//			continue ;
//		params->sem_plock = sem_open(SEM_PLOCK, 0);
//		if (params->sem_plock == SEM_FAILED)
//		{
//			sem_close(params->sem_count);
//			continue ;
//		}
//		break ;
//	}
//	printf("in helper, count: %p\n", params->sem_count);
//	printf("in helper, plock: %p\n", params->sem_plock);
	sem_wait(params->sem_tlock);
//	params->sem_count = sem_open(SEM_COUNT, 0);
//	params->sem_plock = sem_open(SEM_PLOCK, 0);
//	if (params->sem_count == SEM_FAILED || params->sem_plock == SEM_FAILED)
//		kill(0, SIGBUS);
//	printf("in helper, count: %p\n", params->sem_count);
//	printf("in helper, plock: %p\n", params->sem_plock);
	while (true)
	{
		if (sem_wait(params->sem_count) == -1)
			kill(0, SIGTRAP);
		if (kill(params->pids[0], SIGCHLD) == -1)
			break ;
		if (++nbr_philos_done == params->info.philo_max)
		{
			philos_cleanup(params);
			break ;
		}
		if (sem_post(params->sem_plock) == -1)
			kill(0, SIGABRT);
	}
	return (NULL);
}

//pthread_t	main_setup(t_params *params)
void	main_setup(t_params *params)
{
//	pthread_t	thread;
//
//	thread = 0;
	params->sem_forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL,
			S_IRWXU, params->info.philo_max / 2);
	params->sem_print = sem_open(SEM_PRINT, O_CREAT | O_EXCL,
			S_IRWXU, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (philos_cleanup(params),
			ft_error(params, "sem_open-main_setup"));
	if (params->info.track_philo_quotas)
	{
		params->sem_count = sem_open(SEM_COUNT, O_CREAT | O_EXCL,
				S_IRWXU, 0);
		params->sem_plock = sem_open(SEM_PLOCK, O_CREAT | O_EXCL,
				S_IRWXU, 1);
		if (params->sem_count == SEM_FAILED || params->sem_plock == SEM_FAILED)
			return (philos_cleanup(params),
				ft_error(params, "sem_open-main_setup"));
//		if (pthread_create(&thread, NULL, &helper_routine, params) != 0)
//			return (philos_cleanup(params),
//				ft_error(params, "pthread_create-main_setup"), -1);
	}
	//return (thread);
}
