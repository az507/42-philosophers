/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 15:37:08 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_params	*params_create(int argc, char *argv[]);
void		philo_routine(t_params *params);
//pthread_t	main_setup(t_params *params);
void	*helper_routine(void *arg);
void		main_setup(t_params *params);
void		monitor_philos_status(t_params *params);

void	fork_processes(t_params *params)
{
	int	i;

	i = -1;
	while (++i < params->info.philo_max)
	{
		params->philo_id = i + 1;
		params->pids[i] = fork();
		if (params->pids[i] == -1)
		{
			philos_cleanup(params),
			ft_error(params, "fork-fork_processes");
		}
		if (!params->pids[i])
			philo_routine(params);
		kill(params->pids[i], SIGSTOP);
	}
}

int	main(int argc, char *argv[])
{
	t_params	*params;
	pthread_t	thread;

	sems_unlink();
	params = params_create(argc, argv);
	thread = 0;
	pthread_create(&thread, NULL, &helper_routine, params);
	fork_processes(params);
	//thread = main_setup(params);
	main_setup(params);
	if (gettimeofday(&params->start_tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-main", STDERR_FILENO);
//	sem_wait(params->sem_print);
//	printf("in main, addr: %p\n", params->sem_count);
//	sem_post(params->sem_print);
	sem_post(params->sem_tlock);
	kill(0, SIGCONT);
	monitor_philos_status(params);
	if (thread && pthread_join(thread, NULL) != 0)
		;
	params_destroy(params);
	sems_unlink();
}
