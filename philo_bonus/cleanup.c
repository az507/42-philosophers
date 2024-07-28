/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:11:53 by achak             #+#    #+#             */
/*   Updated: 2024/07/28 15:32:02 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_putendl_fd(const char *str, int fd)
{
	while (*str)
		write(fd, str++, 1);
	write(fd, "\n", 1);
}

void	ft_error(t_params *params, const char *error_msg)
{
	ft_putendl_fd(error_msg, STDERR_FILENO);
	params_destroy(params);
	exit(EXIT_FAILURE);
}

void	sem_perror(t_params *params, const char *error)
{
	sem_wait(params->sem_print);
	ft_putendl_fd(error, STDERR_FILENO);
	sem_post(params->sem_print);
}

void	sems_unlink(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_COUNT);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_PLOCK);
}

void	philos_cleanup(t_params *params)
{
	int	i;

	i = 0;
	while (i < params->info.philo_max && params->pids[i] > 0)
	{
		kill(params->pids[i], SIGKILL);
		waitpid(params->pids[i], NULL, 0);
		i++;
	}
}
