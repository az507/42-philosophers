/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/07/30 13:58:07 by achak            ###   ########.fr       */
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
	free(params->pids);
	params->pids = NULL;
	while (true)
		ft_usleep(params, params->info.time_eat);
}

void	philo_routine(t_params *params)
{
	int	times_ate;

	times_ate = 0;
	ft_sem_open(params, 0, (mode_t)0);
	free(params->pids);
	params->pids = NULL;
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
	ft_sem_open(params, 0, (mode_t)0);
	while (true)
	{
		sem_wait(params->sem_count);
		if (kill(params->pids[0], SIGCHLD) == -1)
			break ;
		if (++nbr_philos_done == params->info.philo_max)
		{
			processes_kill(params, SIGKILL);
			break ;
		}
	}
	params_destroy(params);
	exit(EXIT_SUCCESS);
}

static void	print_philo_status(t_params *params, int wstatus, int i)
{
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == EXIT_FAILURE)
			ft_putendl_fd("Critical error occurred", STDERR_FILENO);
		else
			printf("%ld philosopher %d died\n", get_time_ms(params), i + 1);
	}
	else if (WIFSIGNALED(wstatus))
	{
		if (WTERMSIG(wstatus) == SIGABRT)
			ft_putendl_fd("Critical error occurred\n", STDERR_FILENO);
		else
			printf("%ld all philos are done eating\n", get_time_ms(params));
	}
}

void	monitor_routine(t_params *params)
{
	int	wstatus;
	int	i;

	ft_sem_open(params, O_CREAT | O_EXCL, S_IRWXU);
	kill(0, SIGCONT);
	while (true)
	{
		i = -1;
		while (++i < params->info.philo_max)
		{
			wstatus = 0;
			if (!waitpid(params->pids[i], &wstatus, WNOHANG))
				continue ;
			processes_cleanup(params);
			print_philo_status(params, wstatus, i);
			return ;
		}
	}
}
