/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/08/01 11:13:32 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	processes_kill(t_params *params, int sig);

void	ft_usleep(t_params *params, int usleep_amt);
void	philo_think(t_params *params);
void	philo_sleep(t_params *params);
void	philo_eat(t_params *params);

void	one_philo_routine(t_params *params)
{
	while (true)
		ft_usleep(params, params->info.time_eat);
}

void	philo_routine(t_params *params)
{
	int	times_ate;

	times_ate = 0;
	while (true)
	{
		philo_eat(params);
		if (params->info.track_philos_quota
			&& ++times_ate == params->info.eat_max)
			sem_post(params->sem_count);
		philo_sleep(params);
		philo_think(params);
	}
}

void	counter_routine(t_params *params)
{
	int	nbr_philos_done;

	nbr_philos_done = 0;
	while (true)
	{
		sem_wait(params->sem_count);
		if (kill(params->pids[0], 0) == -1)
			break ;
		else if (++nbr_philos_done == params->info.philo_max)
			processes_kill(params, SIGKILL);
	}
	params_destroy(params);
	exit(EXIT_SUCCESS);
}

void	monitor_routine(t_params *params)
{
	int	wstatus;
	int	i;

	i = -1;
	while (true)
	{
		i = -1;
		while (++i < params->info.philo_max)
		{
			wstatus = 0;
			if (!waitpid(params->pids[i], &wstatus, WNOHANG))
				continue ;
			processes_cleanup(params);
			if (WIFEXITED(wstatus))
				printf("%ld philosopher %d died\n",
					get_time_ms(params), i + 1);
			else if (WIFSIGNALED(wstatus))
				printf("%ld all philos are done eating\n",
					get_time_ms(params));
			if (params->info.track_philos_quota)
				sem_post(params->sem_count);
			return ;
		}
	}
}
