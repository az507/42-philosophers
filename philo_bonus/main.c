/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/25 13:47:32 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_params	*params_create(int argc, char *argv[]);
void		philo_routine(t_params *params);

void	processes_cleanup(t_params *params)
{
	int	i;
	
	i = 0;
	while (i <= params->philo_max && params->pids[i] > 0)
	{
		if (kill(params->pids[i], SIGKILL) == -1)
			ft_putendl_fd("kill() error in processes cleanup", STDERR_FILENO);
		if (waitpid(params->pids[i], NULL, 0) == -1)
			ft_putendl_fd("waitpid() error in processes cleanup", STDERR_FILENO);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	int		i;
	t_params	*params;

	i = -1;
	params = params_create(argc, argv);
	while (++i < params->philo_max)
	{
		params->philo_id = i + 1;
		params->pids[i] = fork();
		if (params->pids[i] == -1)
			return (processes_cleanup(params),
				ft_error(params, "fork() error in main"), 1);
		if (!params->pids[i])
			philo_routine(params);
		kill(params->pids[i], SIGSTOP);
	}
	kill(0, SIGCONT);
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday() error in main", STDERR_FILENO);
	params->start_time = tv.tv_sec;
	params->pids[params->philo_max] = fork();
	if (params->pids[params->philo_max] == -1)
		return (processes_cleanup(params),
			ft_error(params, "fork() error in main"), 1);
	if (!params->pids[params->philo_max])
	{
		params_destroy(params);
		while (true)
			;
	}
	kill(params->pids[params->philo_max], SIGSTOP);
	pid_t	pid;
	int	wstatus;
	int	nbr_philos_done_eating = 0;
	bool	break_flag = false;
	while (true)
	{
		i = -1;
		while (++i <= params->philo_max)
		{
			wstatus = 0;
			pid = waitpid(params->pids[i], &wstatus, WNOHANG | WCONTINUED);
			if (!pid)
				continue ;
			else if (pid == (pid_t)-1)
				ft_putendl_fd("waitpid() error in main loop", STDERR_FILENO);
			if (WIFEXITED(wstatus))
			{
				processes_cleanup(params);
				printf("%ld philosopher %d died\n",
					get_time_ms(params->start_time), i + 1);
				break_flag = true;
				break ;
			}
			else if (WIFCONTINUED(wstatus))
			{
				if (++nbr_philos_done_eating == params->philo_max)
				{
					processes_cleanup(params);
					printf("%ld all philosophers are done eating\n",
						get_time_ms(params->start_time));
					break_flag = true;
					break ; // print all philos done eating msg
				}
				kill(params->pids[params->philo_max], SIGSTOP);
			}
		}
		if (break_flag)
			break ;
	}
	params_destroy(params);
	if (sem_unlink(SEM_NAME) == -1)
		ft_error(NULL, "sem_unlink() error in main");
}
