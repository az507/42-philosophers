/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 17:01:10 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sems_unlink(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_COUNT);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_PLOCK);
	sem_unlink(SEM_TLOCK);
}

t_params	*params_create(int argc, char *argv[]);
void		params_routine(t_params *params);

void	paramss_cleanup(t_params *params)
{
	int	i;
	
	i = 0;
	while (i <= params->params_max && params->pids[i] > 0)
	{
		if (kill(params->pids[i], SIGKILL) == -1)
			ft_putendl_fd("kill-paramss_cleanup", STDERR_FILENO);
		if (waitpid(params->pids[i], NULL, 0) == -1)
			ft_putendl_fd("waitpid-paramss_cleanup", STDERR_FILENO);
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
//				sem_wait(params->tlock);
//				term_cond = true;
//				sem_post(params->tlock);
//				sem_post(params->plock);
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

int	main(int argc, char *argv[])
{
	t_params	*params;
	pthread_t	thread;

	sems_unlink();
	params = params_init(argc, argv);

	fork_processes(params);

	params->sem_forks = sem_open(SEM_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, params->params_max / 2);
	params->sem_print = sem_open(SEM_PRINT, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		ft_error(params, "sem_open-params create");

	if (params->setup_fptr)
		thread = (*params->setup_fptr)(params, MAIN);

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
	free(pids);
	sems_unlink();
}
