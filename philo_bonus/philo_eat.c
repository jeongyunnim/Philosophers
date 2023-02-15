/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 15:50:43 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/15 19:52:58 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	pick_up_forks(t_philo *shared, int num)
{
	int	status;

	printf("%d 자식 대기 중\n", num);
	status = sem_wait(shared->forks);
	if (status == -1)
	{
		printf("status: %d\n", status);
		perror("뭔데");
	}
	print_status(shared, num, FORK);
	sem_wait(shared->forks);
	print_status(shared, num, FORK);
}

void	put_down_forks(t_philo *shared)
{
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
	shared->eat_cnt += 1;
	shared->last_eat = get_time();
	split_usleep(shared->conditions->time_to_eat);
	put_down_forks(shared);
	return (0);
}
