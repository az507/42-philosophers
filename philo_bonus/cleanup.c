/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:11:53 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 19:16:34 by achak            ###   ########.fr       */
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
	if (params->sem_print && params->sem_print != SEM_FAILED)
		sem_wait(params->sem_print);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	if (params->sem_print && params->sem_print != SEM_FAILED)
		sem_post(params->sem_print);
	params_destroy(params);
	exit(EXIT_FAILURE);
}

void	sems_unlink(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_COUNT);
	sem_unlink(SEM_PRINT);
}

void	processes_kill(t_params *params, int sig)
{
	int	i;

	i = 0;
	while (i < params->info.philo_max && params->pids[i] > 0)
		kill(params->pids[i++], sig);
	if (params->info.track_philo_quotas && params->pids[i] > 0)
		kill(params->pids[i], sig);
}

void	processes_cleanup(t_params *params)
{
	int	i;

	i = 0;
	processes_kill(params, SIGKILL);
	while (i < params->info.philo_max && params->pids[i] > 0)
		waitpid(params->pids[i++], NULL, 0);
	if (params->info.track_philo_quotas && params->pids[i] > 0)
		waitpid(params->pids[i], NULL, 0);
}
