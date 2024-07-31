/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:54:50 by achak             #+#    #+#             */
/*   Updated: 2024/07/31 20:23:34 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sems_open(t_params *params)
{
	params->sem_forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, S_IRWXU,
			params->info.philo_max / 2);
	params->sem_print = sem_open(SEM_PRINT, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_print == SEM_FAILED)
		ft_error(params, "sem_open-params_create");
	if (params->info.track_philos_quota)
	{
		params->sem_count = sem_open(SEM_COUNT, O_CREAT | O_EXCL, S_IRWXU, 0);
		if (params->sem_count == SEM_FAILED)
			ft_error(params, "sem_open-params_create");
	}
}

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
