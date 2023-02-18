/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 17:35:03 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	pick_up_forks(t_philo *shared, int num)
{
	int	status;

	status = sem_wait(shared->forks);
	if (status == -1)
	{
		printf("status: %d\n", status);
		perror("뭔데");
	}
	print_status(shared, num, FORK);
	sem_wait(shared->forks);
	if (status == -1)
	{
		printf("status: %d\n", status);
		perror("뭔데");
	}
	print_status(shared, num, FORK);
}

void	put_down_forks(t_philo *shared)
{
	//printf("%d put\n", shared->index);
	//printf("%d put\n", shared->index);
	sem_post(shared->forks);
	sem_post(shared->forks);
}

int	eating_spaghetti(t_philo *shared, int num)
{
	pick_up_forks(shared, num);
	if (print_status(shared, num, EAT) == END)
	{
		put_down_forks(shared);
		return (END);
	}
	sem_wait(shared->event_mutex);
	shared->eat_cnt += 1;
	shared->last_eat = get_time() - shared->start;
	sem_post(shared->event_mutex);
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared);
	return (0);
}
