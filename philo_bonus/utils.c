/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 12:54:50 by achak             #+#    #+#             */
/*   Updated: 2024/07/28 15:32:23 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		ft_putendl_fd("gettimeofday-get_time_ms", STDERR_FILENO);
	diff_seconds = tv.tv_sec - params->start_tv.tv_sec;
	diff_microseconds = tv.tv_usec - params->start_tv.tv_usec;
	if (diff_microseconds < 0)
	{
		diff_seconds -= 1;
		diff_microseconds += 1000000;
	}
	return (diff_seconds * 1000 + diff_microseconds / 1000);
}
