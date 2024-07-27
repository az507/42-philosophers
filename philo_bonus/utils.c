/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:54:50 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 11:20:00 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_putendl_fd(const char *str, int fd)
{
	while (*str)
		write(fd, str++, 1);
	write(fd, "\n", 1);
}

void	ft_error(t_philo *philo, const char *error_msg)
{
	ft_putendl_fd(error_msg, STDERR_FILENO);
	philo_destroy(philo);
	exit(EXIT_FAILURE);
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

long	get_time_ms(t_philo *philo)
{
	struct timeval	tv;
	long	diff_seconds;
	long	diff_microseconds;

	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-get_time_ms", STDERR_FILENO);
	diff_seconds = tv.tv_sec - philo->start_tv.tv_sec;
	diff_microseconds = tv.tv_usec - philo->start_tv.tv_usec;
	if (diff_microseconds < 0)
	{
		diff_seconds -= 1;
		diff_microseconds += 1000000;
	}
	return (diff_seconds * 1000 + diff_microseconds / 1000);
}
