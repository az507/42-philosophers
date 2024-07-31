/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/31 12:49:06 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int		ft_atoi(const char *nptr);
void	*ft_malloc(size_t size);

void	sems_unlink(void);

void	one_philo_routine(t_params *params);
void	philo_routine(t_params *params);
void	counter_routine(t_params *params);
void	monitor_routine(t_params *params);

static void	process_fork(t_params *params, void (*process_routine)(t_params *),
		int i)
{
	params->pids[i] = fork();
	if (params->pids[i] == -1)
	{
		processes_cleanup(params);
		ft_error(params, "fork-process_fork");
	}
	if (!params->pids[i])
		(*process_routine)(params);
	kill(params->pids[i], SIGSTOP);
}

static void	processes_create(t_params *params)
{
	int	i;

	i = 0;
	if (params->info.philo_max == 1)
		process_fork(params, &one_philo_routine, i++);
	else
	{
		while (i < params->info.philo_max)
		{
			params->philo_id = i + 1;
			process_fork(params, &philo_routine, i++);
		}
	}
	params->philo_id = INT_MAX;
	if (params->info.track_philos_quota)
		process_fork(params, &counter_routine, i);
}

static void	info_init(t_params *params, int argc, char *argv[])
{
	params->info.philo_max = ft_atoi(argv[1]);
	params->info.time_die = ft_atoi(argv[2]);
	params->info.time_eat = ft_atoi(argv[3]);
	params->info.time_sleep = ft_atoi(argv[4]);
	params->info.track_philos_quota = false;
	if (argc > 5)
	{
		params->info.eat_max = ft_atoi(argv[5]);
		params->info.track_philos_quota = true;
	}
	if (params->info.philo_max <= 0 || params->info.time_die <= 0
		|| params->info.time_eat <= 0 || params->info.philo_max <= 0
		|| (params->info.eat_max <= 0 && argc > 5))
		ft_error(params, "Invalid number as argument");
}

static t_params	*params_create(int argc, char *argv[])
{
	t_params	*params;

	if (argc < 5)
		ft_error(NULL, "Too few arguments");
	params = ft_malloc(sizeof(t_params));
	if (!params)
		ft_error(NULL, "params struct malloc-params_create");
	info_init(params, argc, argv);
	if (params->info.track_philos_quota)
		params->pids = ft_malloc(sizeof(pid_t) * (params->info.philo_max + 1));
	else
		params->pids = ft_malloc(sizeof(pid_t) * params->info.philo_max);
	if (!params->pids)
		ft_error(params, "pids malloc-params_create");
	if (gettimeofday(&params->start_tv, NULL) == -1)
		ft_error(params, "gettimeofday-params_create");
	return (params);
}

int	main(int argc, char *argv[])
{
	t_params	*params;

	sems_unlink();
	params = params_create(argc, argv);
	processes_create(params);
	params->philo_id = 0;
	monitor_routine(params);
	params_destroy(params);
	sems_unlink();
}
