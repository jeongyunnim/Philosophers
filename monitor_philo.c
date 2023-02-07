/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:29:37 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 21:21:46 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	dead_monitor(t_philo *shared, unsigned int i, int num, int die_time)
{
	long	last_eat;

	pthread_mutex_lock(&shared->mutexes[LASTEAT_M]);
	last_eat = shared->last_eat[i % num];
	pthread_mutex_unlock(&shared->mutexes[LASTEAT_M]);
	if (last_eat + die_time < get_time() - shared->start_point)
	{
		print_status(shared, i % num + 1, DEAD);
		return (END);
	}
	return (0);
}

int	eatcnt_monitor(t_philo *shared, int num)
{
	int		i;

	i = 0;
	if (shared->conditions->must_eat == 0)
		return (0);
	pthread_mutex_lock(&shared->mutexes[EATCNT_M]);
	while (i < num)
	{
		if (shared->eat_cnt[i] < shared->conditions->must_eat)
			return (0);
		i++;
	}
	pthread_mutex_unlock(&shared->mutexes[EATCNT_M]);
	return (END);
}

void	*thread_monitoring(void *philo_shared)
{
	t_philo			*shared;
	unsigned int	i;
	int				die_time;
	int				num;
	
	shared = (t_philo *)philo_shared;
	die_time = shared->conditions->time_to_die;
	num = shared->conditions->philo_number;
	// pthread_mutex_unlock(&shared->mutexes[WAIT_M]);
	i = 0;
	while (1)
	{
		if (dead_monitor(shared, i, num, die_time) == END)
			break ;
		if (eatcnt_monitor(shared, num) == END)
			break ;
		usleep(256);
		i++;
	}
	pthread_mutex_lock(&shared->mutexes[END_M]);
	shared->end_flag = END;
	pthread_mutex_unlock(&shared->mutexes[END_M]);
	pthread_mutex_unlock(&shared->mutexes[PRT_M]);
	return (NULL);
}
