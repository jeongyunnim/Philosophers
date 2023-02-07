/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 19:37:00 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	pick_up_forks_even(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	print_status(shared, num, FORK);
	// if (num == 1)
	// {
	// 	pthread_mutex_unlock(&shared->fork_mutex[right_fork]);
	// 	return (END);
	// }
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	shared->fork[right_fork] = 1;
	shared->fork[left_fork] = 1;
	print_status(shared, num, FORK);
	return (0);
}

int	pick_up_forks_odd(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	print_status(shared, num, FORK);
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	shared->fork[right_fork] = 1;
	shared->fork[left_fork] = 1;
	print_status(shared, num, FORK);
	return (0);
}

void	put_down_forks(t_philo *shared, int left_fork, int right_fork)
{
	shared->fork[left_fork] = 0;
	shared->fork[right_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[left_fork]);
	pthread_mutex_unlock(&shared->fork_mutex[right_fork]);
}

int	eating_spaghetti(t_philo *shared, int num, int left_fork, int right_fork)
{
	if (num % 2 == 1)
		pick_up_forks_even(shared, num, left_fork, right_fork);
	else
		pick_up_forks_odd(shared, num, left_fork, right_fork);
	if (print_status(shared, num, EAT) == END)
	{
		put_down_forks(shared, left_fork, right_fork);
		return (END);
	}
	pthread_mutex_lock(&shared->mutexes[LASTEAT_M]);
	shared->last_eat[num - 1] = get_time() - shared->start_point;
	pthread_mutex_unlock(&shared->mutexes[LASTEAT_M]);
	pthread_mutex_lock(&shared->mutexes[EATCNT_M]);
	shared->eat_cnt[num - 1] += 1;
	pthread_mutex_unlock(&shared->mutexes[EATCNT_M]);
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared, left_fork, right_fork);
	return (0);
}
