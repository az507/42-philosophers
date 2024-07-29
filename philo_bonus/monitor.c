/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 19:58:31 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	processes_kill(t_params *params);

static void	print_philo_status(t_params *params, pid_t pid, int wstatus, int i)
{
	if (WIFEXITED(wstatus))
	{
		if (WEXITSTATUS(wstatus) == EXIT_FAILURE)
			ft_putendl_fd("Critical error occurred", 2);
		else
			printf("%ld philosopher %d died\n", get_time_ms(params), i + 1);
	}
	else if (WIFSIGNALED(wstatus) || pid == -1)
	{
		if (WTERMSIG(wstatus) == SIGABRT)
			ft_putendl_fd("Critical error occurred\n", 2);
		else
			printf("%ld all philos are done eating\n", get_time_ms(params));
	}
}

void	monitor_philos_status(t_params *params)
{
	pid_t	pid;
	int		wstatus;
	int		i;

	while (true)
	{
		i = -1;
		while (++i < params->info.philo_max)
		{
			wstatus = 0;
			pid = waitpid(params->pids[i], &wstatus, WNOHANG);
			if (!pid)
				continue ;
			processes_cleanup(params);
			print_philo_status(params, pid, wstatus, i);
			return ;
		}
	}
}

void	helper_routine(t_params *params)
{
	int	nbr_philos_done;

	nbr_philos_done = 0;
	params->sem_count = sem_open(SEM_COUNT, 0);
	params->sem_print = sem_open(SEM_PRINT, 0);
	if (params->sem_count == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (processes_kill(params, SIGABRT),
			ft_error(params, "sem_open-helper_routine"));
	while (true)
	{
		sem_wait(params->sem_count);
		if (kill(params->pids[0], SIGCHLD) == -1)
			break ;
		if (++nbr_philos_done == params->info.philo_max)
			processes_kill(params);
	}
	params_destroy(params);
	exit(EXIT_SUCCESS);
}

void	sem_open_wrapper(t_params *params, mode_t mode, unsigned int value)
{
	if (mode)
		params->sem_forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL,
				S_IRWXU, params->info.philo_max / 2);
		params->sem_print = sem_open(SEM_PRINT, O_CREAT | O_EXCL,
				S_IRWXU, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (processes_cleanup(params),
			ft_error(params, "sem_open-sems_create"));
	if (params->info.track_philo_quotas)
	{
		params->sem_count = sem_open(SEM_COUNT, O_CREAT | O_EXCL,
				S_IRWXU, 0);
		if (params->sem_count == SEM_FAILED)
			return (processes_cleanup(params),
				ft_error(params, "sem_open-sems_create"));
	}
}
//void	sems_open(t_params *params, mode_t mode, unsigned int value)
// TODO: make a sem_open wrapper function that all processes(main, helper, philos) can
// reuse
void	sems_create(t_params *params)
{
	params->sem_forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL,
			S_IRWXU, params->info.philo_max / 2);
	params->sem_print = sem_open(SEM_PRINT, O_CREAT | O_EXCL,
			S_IRWXU, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (processes_cleanup(params),
			ft_error(params, "sem_open-sems_create"));
	if (params->info.track_philo_quotas)
	{
		params->sem_count = sem_open(SEM_COUNT, O_CREAT | O_EXCL,
				S_IRWXU, 0);
		if (params->sem_count == SEM_FAILED)
			return (processes_cleanup(params),
				ft_error(params, "sem_open-sems_create"));
	}
}
