#include "philo_bonus.h"
#include <errno.h>

typedef struct l_params
{
	sem_t	*sem_count;
	sem_t	*sem_plock;
	sem_t	*sem_tlock;
}	b_params;

void	*start_routine(void *arg)
{
	b_params	*params;
	int	counter = 0;

	params = (b_params *)arg;
	sem_wait(params->sem_tlock);
	while (1)
	{
		sem_wait(params->sem_count);
		printf("abc\n");
		//sem_post(params->sem_plock);
		if (++counter == 10)
			break ;
	}
	return (NULL);
}

int	main(void)
{
	b_params	params;
	pthread_t	thread;
	pid_t		pid;
	int		err;

	sem_unlink(SEM_COUNT);
	sem_unlink(SEM_PLOCK);
	sem_unlink(SEM_TLOCK);
	memset(&params, 0, sizeof(params));
	params.sem_tlock = sem_open(SEM_TLOCK, O_CREAT | O_EXCL, S_IRWXU, 0);
	err = pthread_create(&thread, NULL, &start_routine, &params);
	if (err)
	{
		errno = err;
		perror("pthread_create");
	}
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (!pid)
	{
		int	i = -1;
		params.sem_count = sem_open(SEM_COUNT, 0);
		params.sem_plock = sem_open(SEM_PLOCK, 0);
		while (++i < 10)
		{
			sem_post(params.sem_count);
			//sem_wait(params.sem_plock);
			usleep(50000);
		}
		exit(0);
	}
	kill(pid, SIGSTOP);
	params.sem_count = sem_open(SEM_COUNT, O_CREAT | O_EXCL, S_IRWXU, 0);
	params.sem_plock = sem_open(SEM_PLOCK, O_CREAT | O_EXCL, S_IRWXU, 0);
	sem_post(params.sem_tlock);
	kill(pid, SIGCONT);
	if (waitpid(pid, NULL, 0) == -1)
		perror("waitpid");
	err = pthread_join(thread, NULL);
	if (err)
	{
		errno = err;
		perror("pthread_join");
	}
	sem_close(params.sem_count);
	sem_close(params.sem_tlock);
	sem_unlink(SEM_COUNT);
	sem_unlink(SEM_PLOCK);
	sem_unlink(SEM_TLOCK);
}
