/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:29:37 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 16:08:51 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	dead_full_monitor(t_philo *shared, int num)
{
	int	i;
	int	minimum;
	int	must_eat;
	int	die_time;

	i = -1;
	minimum = shared->eat_cnt[0];
	must_eat = shared->conditions->must_eat;
	die_time = shared->conditions->time_to_die;
	pthread_mutex_lock(&shared->mutexes[MNT_M]);
	while (++i < num)
	{
		if (must_eat != 0 && minimum > shared->eat_cnt[i])
			minimum = shared->eat_cnt[i];
		if (shared->last_eat[i % num] + die_time < get_time() - shared->start)
		{
			print_status(shared, i % num + 1, DEAD);
			pthread_mutex_unlock(&shared->mutexes[MNT_M]);
			return (END);
		}
	}
	pthread_mutex_unlock(&shared->mutexes[MNT_M]);
	if (must_eat != 0 && minimum >= must_eat)
		return (END);
	return (0);
}

void	*thread_monitoring(void *philo_shared)
{
	t_philo			*shared;
	int				num;

	shared = (t_philo *)philo_shared;
	num = shared->conditions->philo_number;
	while (1)
	{
		if (dead_full_monitor(shared, num) == END)
			break ;
		usleep(128);
	}
	pthread_mutex_lock(&shared->mutexes[END_M]);
	shared->end_flag = END;
	pthread_mutex_unlock(&shared->mutexes[END_M]);
	pthread_mutex_unlock(&shared->mutexes[PRT_M]);
	return (NULL);
}
