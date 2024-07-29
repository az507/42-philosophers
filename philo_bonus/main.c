/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/29 19:53:20 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_params	*params_create(int argc, char *argv[]);
void		processes_create(t_params *params);

void		philo_routine(t_params *params);
void		helper_routine(t_params *params);

void		main_setup(t_params *params);
void		monitor_philos_status(t_params *params);

int	main(int argc, char *argv[])
{
	t_params	*params;

	sems_unlink();
	params = params_create(argc, argv);
	processes_create(params);
	sems_create(params);
	kill(0, SIGCONT);
	monitor_philos_status(params);
	params_destroy(params);
	sems_unlink();
}
