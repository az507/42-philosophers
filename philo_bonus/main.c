/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/24 19:41:56 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_params	*params_create(int argc, char *argv[]);
void		philo_routine(t_params *params);

void	wait_philos(t_params *params)
{
	int	i;

	i = 0;
	while (++i < params->philo_max)
	{
		if (waitpid(-1, NULL, 0) == -1)
			ft_putendl_fd("waitpid() error in main", STDERR_FILENO);
	}
}

void	kill_philos(t_params *params, pid_t dead_philo_id)
{
	int	j;
	
	j = 0;
	while (j < params->philo_max && params->pids[j] > (pid_t)0)
	{
		if (kill(params->pids[j], SIGKILL) == -1)
			ft_putendl_fd("kill() error in processes cleanup", STDERR_FILENO);
		j++;
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
			return (kill_philos(params), wait_philos(),
				ft_error(params, "fork() error in main"), 1);
		if (!params->pids[i])
			philo_routine(params);
		kill(params->pids[i], SIGSTOP);
	}
	sleep(5);
	kill(0, SIGCONT);
	sleep(5);
	pid_t	pid;
	while (true)
	{
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid > (pid_t)0)
			break ;
	}
	i = -1;
	while (++i < params->philo_max)
	{
		if (params->pids[i] == pid)
		{
			printf("philosopher %d has died\n", i + 1);
			break ;
		}
	}
	// monitor philos, then send kill signal to all if done
	processes_cleanup(params);
	params_destroy(params);
	if (sem_unlink(SEM_NAME) == -1)
		ft_error(NULL, "sem_unlink() error in main");
}
