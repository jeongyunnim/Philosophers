/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/05 16:42:13 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	pick_up_forks(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	shared->fork[right_fork] = 1;
	if (check_dead_or_full(shared) != 0)
	{
		shared->fork[right_fork] = 0;
		pthread_mutex_lock(&shared->fork_mutex[right_fork]);
		return (END);
	}
	print_status(shared, num, FORK);
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	shared->fork[left_fork] = 1;
	if (check_dead_or_full(shared) != 0)
	{
		put_down_forks(shared, left_fork, right_fork);
		return (END);
	}
	print_status(shared, num, FORK);
	return (0);
}

void	put_down_forks(t_philo *shared, int left_fork, int right_fork)
{
	shared->fork[right_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[left_fork]);
	shared->fork[left_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[right_fork]);
}

int	eating_spaghetti(t_philo *shared, int num, int left_fork, int right_fork)
{
	if (pick_up_forks(shared, num, left_fork, right_fork) != 0)
		return (END);
	if (check_dead_or_full(shared) != 0)
	{
		put_down_forks(shared, left_fork, right_fork);
		return (END);
	}
	print_status(shared, num, EAT);
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared, left_fork, right_fork);
	return (0);
}
