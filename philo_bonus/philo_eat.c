/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 21:12:05 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	pick_up_forks(t_philo *shared, int num)
{
	sem_wait(shared->forks);
	print_status(shared, num, FORK);
	sem_wait(shared->forks);
	print_status(shared, num, FORK);
}

void	put_down_forks(t_philo *shared)
{
	sem_post(shared->forks);
	sem_post(shared->forks);
}

void	eating_spaghetti(t_philo *shared, int num)
{
	pick_up_forks(shared, num);
	print_status(shared, num, EAT);
	sem_wait(shared->event_mutex);
	shared->eat_cnt += 1;
	shared->last_eat = get_time() - shared->start;
	sem_post(shared->event_mutex);
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared);
}
