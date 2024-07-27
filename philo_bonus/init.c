/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:52:31 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 19:01:42 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_putendl_fd(const char *str, int fd);
int	ft_atoi(const char *nptr);

static void	info_init(t_params *params, int argc, char *argv[])
{
	params->info.philo_max = ft_atoi(argv[1]);
	params->info.time_die = ft_atoi(argv[2]) * MS_TO_MICROSEC;
	params->info.time_eat = ft_atoi(argv[3]) * MS_TO_MICROSEC;
	params->info.time_sleep = ft_atoi(argv[4]) * MS_TO_MICROSEC;
	params->info.track_philos_quotas = false;
	if (argc > 5)
	{
		params->info.eat_max = ft_atoi(argv[5]);
		params->info.track_philos_quotas = true;
	}
	if (params->info.philo_max <= 0 || params->info.time_die <= 0
		|| params->info.time_eat <= 0 || params->info.params_max <= 0
		|| (params->info.eat_max <= 0 && argc > 5))
		ft_error(params, "Invalid number as argument");
}

t_params	*params_create(int argc, char *argv[])
{
	t_params	*params;

	if (argc < 5)
		ft_error(NULL, "Too few arguments");
	params = ft_malloc(sizeof(t_params));
	if (!params)
		ft_error(NULL, "params struct malloc-params create");
	info_init(params, argc, argv);
	params->pids = ft_malloc(sizeof(pid_t) * params->info.philo_max);
	if (!params->pids)
		ft_error(params, "pids malloc");
	return (params);
}

void	params_destroy(t_params *params)
{
	if (params)
	{
		if (params->pids)
			free(params->pids);
		if (params->sem_forks != SEM_FAILED && params->sem_forks)
		{
			if (sem_close(params->sem_forks) == -1)
				ft_putendl_fd("sem_close-params destroy",
					STDERR_FILENO);
		}
		if (params->sem_count != SEM_FAILED && params->sem_count)
		{
			if (sem_close(params->sem_count) == -1)
				ft_putendl_fd("sem_close-params destroy",
					STDERR_FILENO);
		}
		free(params);
	}
}
