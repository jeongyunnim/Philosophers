/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 17:29:01 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	pick_up_forks_even(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	if (print_status(shared, num, FORK) == END)
	{
		pthread_mutex_unlock(&shared->fork_mutex[right_fork]);
		return (END);
	}
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	shared->fork[right_fork] = 1;
	shared->fork[left_fork] = 1;
	if (print_status(shared, num, FORK) == END)
	{
		put_down_forks(shared, left_fork, right_fork);
		return (END);
	}
	return (0);
}

int	pick_up_forks_odd(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	if (print_status(shared, num, FORK) == END)
	{
		pthread_mutex_unlock(&shared->fork_mutex[left_fork]);
		return (END);
	}
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	shared->fork[right_fork] = 1;
	shared->fork[left_fork] = 1;
	if (print_status(shared, num, FORK) == END)
	{
		put_down_forks(shared, left_fork, right_fork);
		return (END);
	}
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
	int flag;

	flag = 0;
	if (num % 2 == 1)
		flag = pick_up_forks_even(shared, num, left_fork, right_fork);
	else
		flag = pick_up_forks_odd(shared, num, left_fork, right_fork);
	if (flag == END)
		return (END);
	pthread_mutex_lock(&shared->last_eat_mutex[num]);
	shared->last_eat[num - 1] = get_time() - shared->start_point;
	pthread_mutex_unlock(&shared->last_eat_mutex[num]);
	if (print_status(shared, num, EAT) == END)
	{
		put_down_forks(shared, left_fork, right_fork);
		return (END);
	}
	pthread_mutex_lock(&shared->mutexes[EATCNT_M]);
	shared->eat_cnt[num - 1] += 1;
	pthread_mutex_unlock(&shared->mutexes[EATCNT_M]);
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared, left_fork, right_fork);
	return (0);
}
