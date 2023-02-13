/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/13 17:56:21 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	pick_up_forks(t_philo *shared, int num)
{
	sem_wait(&shared->semaphore);
	print_status(shared, num, FORK);
	sem_wait(&shared->semaphore);
	print_status(shared, num, FORK);
}

void	put_down_forks(t_philo *shared)
{
	sem_post(&shared->semaphore);
	sem_post(&shared->semaphore);
}

int	eating_spaghetti(t_philo *shared, int num)
{
	pick_up_fork(shared, num);
	if (print_status(shared, num, EAT) == END)
	shared->eat_cnt += 1;
	shared->last_eat = get_time();
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared);
	return (0);
}
