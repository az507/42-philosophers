/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/07/28 16:47:44 by achak            ###   ########.fr       */
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
			wstatus = 0;
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

static void	*helper_routine(void *arg)
{
	t_params	*params;
	int			nbr_philos_done;
	sem_t	*sem_count;
	sem_t	*sem_plock;

	params = (t_params *)arg;
	nbr_philos_done = 0;
	sem_close(params->sem_count);
	sem_close(params->sem_plock);
	sem_count = sem_open(SEM_COUNT, 0); //
	sem_plock = sem_open(SEM_PLOCK, 0);
	if (params->sem_count == SEM_FAILED || params->sem_plock == SEM_FAILED)
		kill(0, SIGTRAP);
//	sem_wait(params->sem_print);
//	printf("in helper, addr: %p\n", params->sem_count);
//	sem_post(params->sem_print);
	while (true)
	{
		if (sem_wait(sem_count) == -1)
			kill(0, SIGTRAP);
			//sem_perror(params, "sem_wait-helper_routine");
		if (kill(params->pids[0], SIGCHLD) == -1)
			break ;
		if (++nbr_philos_done == params->info.philo_max)
		{
			philos_cleanup(params);
			break ;
		}
		if (sem_post(sem_plock) == -1)
			kill(0, SIGABRT);
			//sem_perror(params, "sem_post-helper_routine");
	}
	return (NULL);
}

pthread_t	main_setup(t_params *params)
{
	pthread_t	thread;

	thread = 0;
	params->sem_forks = sem_open(SEM_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, params->info.philo_max / 2);
	params->sem_print = sem_open(SEM_PRINT, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (philos_cleanup(params),
			ft_error(params, "sem_open-main_setup"), -1);
	if (params->info.track_philo_quotas)
	{
		params->sem_count = sem_open(SEM_COUNT, O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 0);
		params->sem_plock = sem_open(SEM_PLOCK, O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
		if (params->sem_count == SEM_FAILED || params->sem_plock == SEM_FAILED)
			return (philos_cleanup(params),
				ft_error(params, "sem_open-main_setup"), -1);
		if (pthread_create(&thread, NULL, &helper_routine, params) != 0)
			return (philos_cleanup(params),
				ft_error(params, "pthread_create-main_setup"), -1);
	}
	return (thread);
}
