/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:52:31 by achak             #+#    #+#             */
/*   Updated: 2024/07/26 18:30:40 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_putendl_fd(const char *str, int fd);
int	ft_atoi(const char *nptr);

static void	philo_info_init(t_params *params, int argc, char *argv[])
{
	params->philo_max = ft_atoi(argv[1]);
	params->time_die = ft_atoi(argv[2]) * MS_TO_MICROSEC;
	params->time_eat = ft_atoi(argv[3]) * MS_TO_MICROSEC;
	params->time_sleep = ft_atoi(argv[4]) * MS_TO_MICROSEC;
	//params->track_times_eaten = false;
	if (argc > 5)
	{
		params->eat_max = ft_atoi(argv[5]);
		//params->track_times_eaten = true;
	}
	if (params->philo_max <= 0 || params->time_die <= 0
		|| params->time_eat <= 0 || params->philo_max <= 0
		|| (params->eat_max <= 0 && argc > 5))
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
	philo_info_init(params, argc, argv);
	params->pids = ft_malloc(sizeof(pid_t) * (params->philo_max + 1));
	if (!params->pids)
		ft_error(params, "pids arr malloc-params create");
//	if (sem_close(params->sem_forks) == -1)
//		perror("sem_close-params_create");
//	if (sem_close(params->sem_lock) == -1)
//		perror("sem_close-params_create");
//	params->sem_forks = NULL;
//	params->sem_lock = NULL;
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
		if (params->sem_lock != SEM_FAILED && params->sem_lock)
		{
			if (sem_close(params->sem_lock) == -1)
				ft_putendl_fd("sem_close-params destroy",
					STDERR_FILENO);
		}
		free(params);
	}
}
