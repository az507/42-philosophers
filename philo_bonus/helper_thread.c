/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_thread.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 17:01:28 by achak             #+#    #+#             */
/*   Updated: 2024/07/27 17:31:28 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		if (++nbr_philos_done == params->info.philos_max)
		{
			philos_cleanup(params);
			return (NULL);
		}
	}
	return (NULL);
}

pthread_t	helper_thread_setup(t_philo *philo)
{
	t_helper	helper;
	pthread_t	thread;

	helper.philo = philo;
	helper.sem_tlock = sem_open(SEM_TLOCK, O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP, S_IWGRP, 1);
	helper.term_cond = false;
	if (pthread_create(&thread, NULL, &helper_routine, &helper) != 0)
		return (philos_cleanup(philo),
			ft_error(philo, "pthread_create-main"), 1);
	return (thread);
}

pthread_t	addon_setup(t_philo *philo, int process_type)
{
	pthread_t	thread;

	thread = 0;
	if (process_type == PHILO)
	{
		philo->sem_count = sem_open(SEM_COUNT, 0);
		philo->sem_plock = sem_open(SEM_PLOCK, 0);
	}
	else if (process_type == MAIN)
	{
		philo->sem_count = sem_open(SEM_COUNT, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP, S_IWGRP, philo->info.philo_max);
		philo->sem_plock = sem_open(SEM_PLOCK, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP, S_IWGRP, 1);
		thread = helper_thread_setup(philo);
	}
	if (philo->sem_count == SEM_FAILED || philo->sem_plock == SEM_FAILED)
		sem_perror(philo, "sem_open-philo_extra_init");
	return (thread);
}

t_addon	*addon_init(void)
{
	t_addon	*addon;

	addon = ft_malloc(sizeof(t_addon));
	if (!addon)
		return (NULL);
	addon->setup_fptr = &addon_setup;
	addon->track_fptr = &philo_track;
	return (addon);
}

void	addon_destroy(t_addon *addon)
{
	if (addon)
	{
		if (addon->sem_count)
			sem_close(addon->sem_count);
		if (addon->sem_plock)
			sem_close(addon->sem_plock);
		free(addon);
	}
}
