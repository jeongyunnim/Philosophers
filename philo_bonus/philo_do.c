/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_do.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:44 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 16:02:32 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	*philo_do(t_philo *shared)
{
	int	num;

	num = shared->index;
	while (1)
	{
		if (eating_spaghetti(shared, num) != 0)
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
	long	time_stamp;

	time_stamp = get_time() - shared->start;
	if (end_check(shared) == END)
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
	return (0);
}
