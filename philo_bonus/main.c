/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/23 20:21:54 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_putendl_fd(char *str, int fd)
{
	while (*str)
		write(fd, *str++, 1);
	write(fd, "\n", 1);
}

void	params_destroy(t_params *params)
{
	if (params)
		;
}

void	ft_error(t_params *params, const char *error_msg)
{
	ft_putendl_fd(error_msg, STDERR_FILENO);
	params_destroy(params);
	exit(EXIT_FAILURE);
}

t_params	*params_create(int argc, char *argv[])
{
	t_params	*params;

	params = malloc(sizeof(t_params));
	if (!params)
		ft_error(NULL, "params struct malloc");
}

int	main(int argc, char *argv[])
{
	t_params	*params;

	params = params_create(argc, argv);
}
