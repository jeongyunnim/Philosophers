/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:44 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/17 20:49:50 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	*philo_do(t_philo *shared)
{
	while (1)
	{
		if (eating_spaghetti(shared, shared->index) != 0)
			break ;
		if (sleeping(shared, shared->index) != 0)
			break ;
		if (thinking(shared, shared->index) != 0)
			break ;
	}
	return (NULL);
}

int	print_status(t_philo *shared, int num, char status)
{
	long	time_stamp;

	if (sem_wait(shared->print_mutex) == -1)
		perror("print 실패");
	time_stamp = get_time() - shared->start;
	if (shared->end_flag == END)
		return (END);
	if (status == EAT)
		printf("%ld %d is eating\n", time_stamp, num);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", time_stamp, num);
	else if (status == DEAD)
		printf("%ld %d died\n", time_stamp, num);
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", time_stamp, num);
	sem_post(shared->print_mutex);
	return (0);
}
