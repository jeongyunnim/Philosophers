/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:44 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 13:26:54 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	set_fork(int num, int philo_num, int *left, int *right)
{
	*right = num - 1;
	if (num == 1)
		*left = philo_num - 1;
	else
		*left = num - 2;
}

void	*philo_do(void *philo_shared)
{
	t_philo	*shared;
	int		num;
	int		left_fork;
	int		right_fork;

	shared = (t_philo *)philo_shared;
	pthread_mutex_lock(&shared->mutexes[INDEX_M]);
	shared->index++;
	num = shared->index;
	pthread_mutex_unlock(&shared->mutexes[INDEX_M]);
	set_fork(num, shared->conditions->philo_number, &left_fork, &right_fork);
	while (1)
	{
		if (eating_spaghetti(shared, num, left_fork, right_fork) != 0)
			break ;
	 	if (sleeping(shared, num) != 0)
		 	break ;
		if (thinking(shared, num) != 0)
			break ;
	}
	return (NULL);
}

int	print_status(t_philo *shared, int num, char status)
{
	pthread_mutex_lock(&shared->mutexes[PRT_M]);
	if (end_check(shared) == END)
	{
		pthread_mutex_unlock(&shared->mutexes[PRT_M]);
		return (END);
	}
	if (status == EAT)
		printf("%ld %d is eating\n", get_time() - shared->start_point, num);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", get_time() - shared->start_point, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", get_time() - shared->start_point, num);
	else if (status == DEAD)
	{
		printf("%ld %d died\n", get_time() - shared->start_point, num);
		return (0);
	}
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", get_time() - shared->start_point, num);
	pthread_mutex_unlock(&shared->mutexes[PRT_M]);
	return (0);
}
