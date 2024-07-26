/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 20:12:13 by achak             #+#    #+#             */
/*   Updated: 2024/07/26 19:47:30 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_params	*params_create(int argc, char *argv[]);
void		philo_routine(t_params *params);

void	processes_cleanup(t_params *params)
{
	int	i;
	
	i = 0;
	while (i <= params->philo_max && params->pids[i] > 0)
	{
		if (kill(params->pids[i], SIGKILL) == -1)
			ft_putendl_fd("kill-processes_cleanup", STDERR_FILENO);
		if (waitpid(params->pids[i], NULL, 0) == -1)
			ft_putendl_fd("waitpid-processes_cleanup", STDERR_FILENO);
		i++;
	}
}

void	*helper_routine(void *arg)
{
	t_params	*params;
	//int		wstatus;
	int		nbr_philos_done;

	// Maybe need to call sem_open for sem_lock in helper thread, to resolve
	// drd/helgrind semaphore error.
	params = (t_params *)arg;
	nbr_philos_done = 0;
//	params->sem_lock = sem_open(SEM_LOCK, 0);
//	if (params->sem_lock == SEM_FAILED)
//		perror("sem_open-helper_routine");
	if (sem_wait(params->sem_lock) == -1)
		perror("\tsem_wait-helper_routine");
	while (true)
	{
//		wstatus = 0;
//		if (waitpid(params->pids[0], &wstatus, WUNTRACED) == -1)
//			ft_putendl_fd("waitpid-helper_routine", STDERR_FILENO);
//		sem_wait(params->sem_lock);
//		if (WIFSTOPPED(wstatus))
//		{
//			if (++nbr_philos_done == params->philo_max)
//				return (processes_cleanup(params),
//					printf("%ld all philos are done eating\n",
//					get_time_ms(params->start_time)), NULL);
//			kill(params->pids[0], SIGCONT);
//		}
//		else if (WIFSIGNALED(wstatus))
//			return (NULL);
//		sem_post(params->sem_lock);
		if (sem_wait(params->sem_lock) == -1)
			perror("\tsem_wait");
		if (++nbr_philos_done == params->philo_max)
		{
			processes_cleanup(params);
			if (sem_wait(params->sem_print) == -1)
				perror("sem_wait-helper_routine");
			printf("%ld all philos are done eating\n",
				get_time_ms(params->start_time));
			if (sem_post(params->sem_print) == -1)
				perror("sem_post-helper_routine");
			return (NULL);
		}
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	int		i;
	t_params	*params;

	//printf("--->%d\n", SEM_VALUE_MAX);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_LOCK);
	sem_unlink(SEM_PRINT);
	i = 0;
	params = params_create(argc, argv);
	params->pids[0] = fork();
	if (params->pids[0] == -1)
		return (processes_cleanup(params),
			ft_error(params, "fork-main"), 1);
	if (!params->pids[0])
	{
		params_destroy(params);
		while (true)
			;
	}
	while (++i <= params->philo_max)
	{
		params->philo_id = i;
		params->pids[i] = fork();
		if (params->pids[i] == -1)
			return (processes_cleanup(params),
				ft_error(params, "fork-main"), 1);
		if (!params->pids[i])
			philo_routine(params);
		kill(params->pids[i], SIGSTOP);
	}
	params->sem_forks = sem_open(SEM_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, params->philo_max);
	params->sem_lock = sem_open(SEM_LOCK, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
	params->sem_print = sem_open(SEM_PRINT, O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);
	if (params->sem_forks == SEM_FAILED || params->sem_lock == SEM_FAILED
		|| params->sem_print == SEM_FAILED)
		ft_error(params, "sem_open-params create");
//	sem_close(params->sem_forks);
//	params->sem_forks = NULL;

	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		ft_putendl_fd("gettimeofday-main", STDERR_FILENO);
	params->start_time = tv.tv_sec;

	pthread_t	tid;

	if (pthread_create(&tid, NULL, &helper_routine, params) != 0)
		return (processes_cleanup(params),
			ft_error(params, "pthread_create-main"), 1);

	kill(0, SIGCONT);
//	sem_close(params->sem_lock);
//	params->sem_lock = NULL;

	int	wstatus;
	bool	break_flag = false;
	while (true)
	{
		i = 0;
		while (++i <= params->philo_max)
		{
			wstatus = 0;
			if (!waitpid(params->pids[i], &wstatus, WNOHANG))
				continue ;
			if (WIFEXITED(wstatus))
			{
				processes_cleanup(params);
				if (sem_wait(params->sem_print) == -1)
					perror("sem_wait-helper_routine");
				printf("%ld philosopher %d died",
					get_time_ms(params->start_time), i);
				if (sem_post(params->sem_print) == -1)
					perror("sem_post-helper_routine");
			}
			else if (!WIFSIGNALED(wstatus))
				continue ;
			break_flag = true;
			break ;
		}
		if (break_flag)
			break ;
	}
	if (pthread_join(tid, NULL) != 0)
		ft_putendl_fd("pthread_join-main", STDERR_FILENO);
	if (sem_close(params->sem_print) == -1)
		perror("sem_close-main");
	params_destroy(params);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_LOCK);
	sem_unlink(SEM_PRINT);
}
