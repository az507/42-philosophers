/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 11:28:06 by achak             #+#    #+#             */
/*   Updated: 2024/04/25 19:04:41 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//void	print_message(t_list *queue_node, long start_time)
void	print_message(t_list *queue_node)
{
//	if (queue_node->state == FORK)
//		printf("%ld philosopher %d has taken a fork\n",
//			get_time_ms() - start_time, queue_node->philo_nbr);
//	else if (queue_node->state == EAT)
//		printf("%ld philosopher %d is eating\n",
//			get_time_ms() - start_time, queue_node->philo_nbr);
//	else if (queue_node->state == SLEEP)
//		printf("%ld philosopher %d is sleeping\n",
//			get_time_ms() - start_time, queue_node->philo_nbr);
//	else if (queue_node->state == THINK)
//		printf("%ld philosopher %d is thinking\n",
//			get_time_ms() - start_time, queue_node->philo_nbr);
//	else if (queue_node->state == DEAD)
//		printf("%ld philosopher %d died\n",
//			get_time_ms() - start_time, queue_node->philo_nbr);
	if (queue_node->state == FORK)
		printf("%ld philosopher %d has taken a fork\n",
			queue_node->timestamp, queue_node->philo_nbr);
	else if (queue_node->state == EAT)
		printf("%ld philosopher %d is eating\n",
			queue_node->timestamp, queue_node->philo_nbr);
	else if (queue_node->state == SLEEP)
		printf("%ld philosopher %d is sleeping\n",
			queue_node->timestamp, queue_node->philo_nbr);
	else if (queue_node->state == THINK)
		printf("%ld philosopher %d is thinking\n",
			queue_node->timestamp, queue_node->philo_nbr);
	else if (queue_node->state == DEAD)
		printf("%ld philosopher %d died\n",
			queue_node->timestamp, queue_node->philo_nbr);
}
/*
int	print_global_queue(t_shared *shared_args)
{
	t_list	*temp;

	pwrap(pthread_mutex_lock(&shared_args->queue_mutex), 6);
	temp = shared_args->global_queue;
	while (temp)
	{
		print_message(temp);
		if (temp->state == DEAD)
		{
			dump_whole_queue(shared_args);
			pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
			return (1);
		}
		temp = temp->next;
	}
	dump_whole_queue(shared_args);
	pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
	return (0);
}
*/

void	shift_queue_to_print(t_shared *shared_args)
{
	t_list	*temp;

	temp = NULL;
	pwrap(pthread_mutex_lock(&shared_args->queue_mutex), 6);
	if (shared_args->global_queue == NULL)
	{
		pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
		return ;
	}
	if (shared_args->print_queue == NULL)
		shared_args->print_queue = shared_args->global_queue;
	else
	{
		temp = shared_args->print_queue;
		shared_args->print_queue = shared_args->global_queue;
		shared_args->tail->next = temp;
	}
	shared_args->global_queue = NULL;
	shared_args->tail = NULL;
	pwrap(pthread_mutex_unlock(&shared_args->queue_mutex), 7);
}

int	check_if_terminate(t_shared *shared_args, t_list *temp)
{	
	if (temp->state == DEAD)
		return (1);
	else if (temp->state == FULL)
	{
		shared_args->full_philos_nbr++;
		if (shared_args->full_philos_nbr == shared_args->philo_max)
		{
			printf("%ld all philosophers have eaten\n",
				get_time_ms() - shared_args->start_time);
			return (1);
		}
	}
	return (0);
}

int	print_out_queue(t_shared *shared_args)
{
	t_list	*temp;

	if (shared_args->print_queue == NULL)
		return (0);
	temp = shared_args->print_queue;
	while (temp->next)
	{
		//print_message(temp, shared_args->start_time);
		print_message(temp);
		if (check_if_terminate(shared_args, temp) == 1)
			return (1);
		temp = temp->next;
	}
	if (temp)
	{
		//print_message(temp, shared_args->start_time);
		print_message(temp);
		if (check_if_terminate(shared_args, temp) == 1)
			return (1);
	}
	dump_whole_queue(shared_args, temp);
	return (0);
}

void	dump_whole_queue(t_shared *shared_args, t_list *print_tail)
{
	t_list	*temp;

	temp = NULL;
	pwrap(pthread_mutex_lock(&shared_args->dump_mutex), 6);
	if (shared_args->print_queue == NULL)
	{
		pwrap(pthread_mutex_unlock(&shared_args->dump_mutex), 7);
		return ;
	}
	if (shared_args->dump)
		temp = shared_args->dump;
	shared_args->dump = shared_args->print_queue;
	if (print_tail)
		print_tail->next = temp;
	shared_args->print_queue = NULL;
	pwrap(pthread_mutex_unlock(&shared_args->dump_mutex), 7);
}

void	*print_routine(void *arg)
{
	t_philo **philo_arr;
	t_shared	*shared_args;

	philo_arr = (t_philo **)arg;
	shared_args = (*philo_arr)->shared_args;
	wait_sync_start(shared_args);
	while (1)
	{
		//usleep(50000);
		shift_queue_to_print(shared_args);
		if (print_out_queue(shared_args) == 1)
		{
//			t_list	*temp = shared_args->print_queue;
//			printf("in print routine: %p\n", shared_args->global_queue);
//			printf("in print routine: %p\n", shared_args->tail);
//			printf("in print routine: %p\n", shared_args->print_queue);
//			printf("in print routine: %p\n", shared_args->dump);
//			printf("in print routine: %p\n", shared_args->free_queue);
//			while (temp)
//			{
//				printf("shared_args->print_queue->state = %d\n",
//					temp->state);
//				temp = temp->next;
//			}
			pwrap(pthread_mutex_lock(&shared_args->flag_mutex), 6);
			shared_args->loop_flag = 2;
			pwrap(pthread_mutex_unlock(&shared_args->flag_mutex), 7);
			break ;
		}
	}
	return (NULL);
}
