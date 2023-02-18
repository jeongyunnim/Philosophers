/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_do.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:44 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:37:56 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	child_process_do(t_philo *shared)
{
	generate_thread(shared);
	philo_do(shared);
	free_structure(shared);
}

void	*philo_do(t_philo *shared)
{
	int	num;

	num = shared->index + 1;
	while (1)
	{
		eating_spaghetti(shared, num);
		sleeping(shared, num);
		thinking(shared, num);
	}
	return (NULL);
}

int	print_status(t_philo *shared, int num, char status)
{
	long	time_stamp;

	sem_wait(shared->print_mutex);
	time_stamp = get_time() - shared->start;
	if (status == EAT)
		printf("%ld %d is eating\n", time_stamp, num);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", time_stamp, num);
	else if (status == DEAD)
	{
		printf("%ld %d died\n", time_stamp, num);
		return (END);
	}
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", time_stamp, num);
	sem_post(shared->print_mutex);
	return (0);
}
