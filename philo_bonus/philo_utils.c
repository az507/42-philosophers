/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:07:03 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 13:30:02 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sem_printf(t_params *params, const char *str)
{
	long	current_time;

	current_time = get_time_ms(params);
	sem_wait(params->sem_print);
	printf("%ld philosopher %d %s\n", current_time,
		params->philo_id, str);
	if (str[0] == 'h')
		printf("%ld philosopher %d %s\n", current_time,
			params->philo_id, str);
	sem_post(params->sem_print);
}

void	ft_usleep(t_params *params, int usleep_amt)
{
	long	time_of_death;
	long	current_time;

	time_of_death = params->time_last_ate + params->info.time_die;
	current_time = get_time_ms(params);
	if (current_time + usleep_amt < time_of_death)
		usleep(usleep_amt * MS_TO_MICROSEC);
	else
	{
		usleep((time_of_death - current_time) * MS_TO_MICROSEC);
		params_destroy(params);
		exit(EXIT_SUCCESS);
	}
}

void	philo_setup(t_params *params)
{
	params->sem_forks = sem_open(SEM_FORKS, 0);
	params->sem_print = sem_open(SEM_PRINT, 0);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		return (ft_putendl_fd("sem_open-philo_setup", STDERR_FILENO),
			(void)kill(0, SIGTRAP));
	if (params->info.track_philo_quotas)
	{
		params->sem_count = sem_open(SEM_COUNT, 0);
		params->sem_plock = sem_open(SEM_PLOCK, 0);
		if (params->sem_count == SEM_FAILED || params->sem_plock == SEM_FAILED)
			sem_perror(params, "sem_open-philo_setup");
	}
	if (gettimeofday(&params->start_tv, NULL) == -1)
		sem_perror(params, "gettimeofday-philo_setup");
//	free(params->pids);
//	params->pids = NULL;
}
