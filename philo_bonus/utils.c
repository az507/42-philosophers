/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:54:50 by achak             #+#    #+#             */
/*   Updated: 2024/07/31 13:53:12 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	processes_kill(t_params *params, int sig);

void	ft_sem_open(t_params *params, int oflag, mode_t mode)
{
	if (params->philo_id <= params->info.philo_max)
	{
		params->sem_forks = sem_open(SEM_FORKS, oflag);
		params->sem_print = sem_open(SEM_PRINT, oflag);
		if (params->info.track_philos_quota)
			params->sem_count = sem_open(SEM_COUNT, oflag);
	}
	else if (params->philo_id == INT_MAX)
	{
		dprintf(2, "in here\n");
		params->sem_count = sem_open(SEM_COUNT, oflag);
		params->sem_print = sem_open(SEM_PRINT, oflag);
//		sem_close(params->sem_count);
//		params->sem_count = SEM_FAILED;
	}
	else
	{
		params->sem_forks = sem_open(SEM_FORKS, oflag, mode,
				params->info.philo_max / 2);
		params->sem_print = sem_open(SEM_PRINT, oflag, mode, 1);
		if (params->info.track_philos_quota)
			params->sem_count = sem_open(SEM_COUNT, oflag, mode, 0);
	}
	if (params->sem_forks == SEM_FAILED)
		dprintf(2, "\tA\n");
	if (params->sem_print == SEM_FAILED)
		dprintf(2, "\tB, %d\n", getpid());
	if (params->sem_count == SEM_FAILED)
		dprintf(2, "\tC\n");
	if ((params->philo_id != INT_MAX
		&& (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED))
		|| (params->philo_id
		&& params->info.track_philos_quota && params->sem_count == SEM_FAILED))
	{
		perror("sem_open");
		//kill(0, SIGTRAP);
		if (!params->philo_id)
		{
			dprintf(2, "\toption 1\n");
			processes_cleanup(params);
		}
		else if (params->philo_id == INT_MAX)
		{
			dprintf(2, "\toption 2\n");
			processes_kill(params, SIGABRT);
		}
		//ft_error(params, "sem_open-ft_sem_open");
		params_destroy(params);
		exit(EXIT_FAILURE);
	}
}

//void	sems_open(t_params *params)
//{
//	if (params->philo_id >= params->info.phlo_max)
//	{
//		ft_sem_open(params, 0, (mode_t)0);
//		ft_sem_open(params, 0, (mode_t)0);
//		if (params->info.track_philos_quota)
//			ft_sem_open(params, 0, (mode_t)0);
//	}
//	else if (params->philo_id == INT_MAX)
//		ft_sem_open(params, 0, (mode_t)0);
//	else
//	{
//		ft_sem_open(params, 0, 
//}

void	ft_putendl_fd(const char *str, int fd)
{
	while (*str)
		write(fd, str++, 1);
	write(fd, "\n", 1);
}

int	ft_atoi(const char *nptr)
{
	int		check;
	long	result;

	check = 1;
	result = 0;
	while ((*nptr == 32 || (*nptr >= 9 && *nptr <= 13)) && *nptr)
		nptr++;
	if (*nptr == '-')
		check = -1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (*nptr != '\0' && *nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
	}
	return ((int)(result * check));
}

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	return (memset(ptr, 0, size));
}

long	get_time_ms(t_params *params)
{
	struct timeval	tv;
	long			diff_seconds;
	long			diff_microseconds;

	if (gettimeofday(&tv, NULL) == -1)
		ft_error(params, "gettimeofday-get_time_ms");
	diff_seconds = tv.tv_sec - params->start_tv.tv_sec;
	diff_microseconds = tv.tv_usec - params->start_tv.tv_usec;
	return (diff_seconds * 1000 + diff_microseconds / 1000);
}
