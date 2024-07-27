/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 19:01:40 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_params	*params_create(int argc, char *argv[]);
void		params_routine(t_params *params);

void	sems_unlink(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_COUNT);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_PLOCK);
	sem_unlink(SEM_TLOCK);
}

void	paramss_cleanup(t_params *params)
{
	int	i;
	
	i = 0;
	while (i <= params->params_max && params->pids[i] > 0)
	{
		if (kill(params->pids[i], SIGKILL) == -1)
			ft_putendl_fd("kill-philos_cleanup", STDERR_FILENO);
		if (waitpid(params->pids[i], NULL, 0) == -1)
			ft_putendl_fd("waitpid-philos_cleanup", STDERR_FILENO);
		i++;
	}
}

void	monitor_philos_status(t_params *params)
{
	int	wstatus;

	while (true)
	{
		i = -1;
		while (++i < params->params_max)
		{
			wstatus = 0;
			if (!waitpid(params->pids[i], &wstatus, WNOHANG)
				|| (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus)))
				continue ;
			else if (WIFEXITED(wstatus))
			{
				paramss_cleanup(params);
				params->params_id = i + 1;
				sem_printf(params, DEAD);
			}
			else if (WIFSIGNALED(wstatus))
				printf("%ld all paramss are done eating\n",
					get_time_ms(params));
			return ;
		}
	}
}

void	fork_processes(t_params *params)
{
	int	i;

	params->pids = ft_malloc(sizeof(pid_t) * params->info.params_max);
	if (!params->pids)
		ft_error(params, "pids malloc-main");
	i = -1;
	while (++i < params->info.params_max)
	{
		params->params_id = i + 1;
		params->pids[i] = fork();
		if (params->pids[i] == -1)
			return (philos_cleanup(params),
				ft_error(params, "fork-main"), 1);
		if (!params->pids[i])
			params_routine(params, params->pids);
		kill(params->pids[i], SIGSTOP);
	}
}

void	main_setup(t_params *params)
{
	pthread_t	thread;

	thread = 0;
	params->sem_forks = sem_open(SEM_FORKS, O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, params->info.philo_max / 2); 
	params->sem_print = sem_open(SEM_PRINT, O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1); 
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		ft_error(params, "sem_open-main_setup");
	if (params->track_philos_quota)
	{
		params->sem_count = sem_open(SEM_COUNT, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1); 
		params->sem_plock = sem_open(SEM_PLOCK, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1); 
		if (params->sem_count == SEM_FAILED || params->sem_plock == SEM_FAILED)
			ft_error(params, "sem_open-main_setup");
		if (pthread_create(&thread, NULL, &helper_routine, params) != 0)
			ft_putendl_fd("pthread_create-main_setup", STDERR_FILENO);
	}
	return (thread);
}

// TODO: HANDLE ALL THE ERROR RETURNS AND PROPER RESOURCE CLEANUP PROCEDURES - currently
// in a mess

int	main(int argc, char *argv[])
{
	t_params	*params;
	pthread_t	thread;

	sems_unlink();
	params = params_init(argc, argv);

	fork_processes(params);

	thread = main_setup(params);

	if (gettimeofday(&params->start_tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-main", STDERR_FILENO);

	kill(0, SIGCONT);
	// from this point on, need to wrap printf and putendl with sem_print

	monitor_philos_status(params);

	if (thread && pthread_join(thread, NULL) != 0)
		; // **pthread functions dont set ERRNO
	if (sem_close(params->sem_print) == -1)
		sem_perror(params->sem_print, "sem_close-main");
	params_destroy(params);
	sems_unlink();
}
