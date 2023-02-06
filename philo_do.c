/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:44 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/06 19:42:31 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*philosopher_do_something(void *philo_shared)
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

	right_fork = num - 1;
	if (num == 1)
		left_fork = shared->conditions->philo_number - 1;
	else
		left_fork = num - 2;

	pthread_mutex_lock(&shared->mutexes[WAIT_M]);
	pthread_mutex_unlock(&shared->mutexes[WAIT_M]);
	if (num % 2 == 0)
		usleep(50);
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
