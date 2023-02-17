/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:29:37 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/17 20:39:37 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	dead_full_monitor(t_philo *shared, int num)
{
	int	must_eat;
	int	die_time;


	sem_wait(shared->event_mutex);
	must_eat = shared->conditions->must_eat;
	die_time = shared->conditions->time_to_die;
	if (shared->last_eat + die_time < get_time() - shared->start)
	{
		print_status(shared, num, DEAD);
		return (END);
	}
	if (must_eat != 0 && shared->eat_cnt >= must_eat)
		return (END);
	sem_post(shared->event_mutex);
	return (0);
}

void	*philo_monitoring(void *philo_shared)
{
	t_philo			*shared;
	int				num;

	shared = (t_philo *)philo_shared;
	num = shared->index;
	while (1)
	{
		if (dead_full_monitor(shared, num) == END)
			break ;
		usleep(128);
	}
	shared->end_flag = END;
	return (NULL);
}

void	generate_thread(t_philo *shared)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, philo_monitoring, shared);
	pthread_detach(monitor);
}