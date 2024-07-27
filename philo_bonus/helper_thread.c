/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 18:58:45 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// if params->pids[i] has been terminated and waited on already, calling kill()
// here will return -1 (there may be a more efficient method of checking if
// any philo process has ended themselves, maybe just checking a single philo is
// enough actually)

void	*helper_routine(void *arg)
{
	t_params	*params;
	int		nbr_philos_done;

	params = (t_params *)arg;
	nbr_philos_done = 0;
	if (sem_wait(params->sem_count) == -1)
		ft_putendl_fd("\tsem_wait-helper_routine", STDERR_FILENO);
	while (true)
	{
		if (sem_wait(params->sem_count) == -1)
			ft_putendl_fd("\tsem_wait", STDERR_FILENO);
		if (kill(params->pids[0], SIGCHLD) == -1)
			break ;
		if (++nbr_philos_done == params->info.philos_max)
		{
			philos_cleanup(params);
			break ;
		}
		if (sem_post(params->sem_plock) == -1)
			sem_perror(params, "sem_post-helper_routine");
	}
	return (NULL);
}
