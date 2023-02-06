/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:29:37 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/06 21:38:50 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int dead_monitor(t_philo *shared, unsigned int i, int num, int die_time)
{
	int passed_time;

	pthread_mutex_lock(&shared->last_eat_mutex[i % num]);
	passed_time = shared->last_eat[i % num];
	pthread_mutex_unlock(&shared->last_eat_mutex[i % num]);

	if (passed_time > die_time + get_time())
	{
		print_status(shared, i % num + 1, DEAD);
		printf("passed: %d, die_time: %d", passed_time, die_time);
		pthread_mutex_lock(&shared->mutexes[END_M]);
		shared->end_flag = END;
		pthread_mutex_unlock(&shared->mutexes[END_M]);
		return (END);
	}
	return (0);
}

int	eatcnt_monitor(t_philo *shared, int num)
{
	int		i;
	char	flag;

	i = 0;
	flag = 1;
	if (shared->conditions->must_eat == 0)
		return (0);
	while (i < num)
	{
		if (shared->eat_cnt[i] < shared->conditions->must_eat)
		{
			flag = 0;
			break ;
		}
		i++;
	}
	if (flag == 1)
	{
		pthread_mutex_lock(&shared->mutexes[END_M]);
		shared->end_flag = END;
		pthread_mutex_unlock(&shared->mutexes[END_M]);
		return (END);
	}
	return (0);
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
	pthread_mutex_unlock(&shared->mutexes[WAIT_M]);
	i = 0;
	while (1)
	{
		if (dead_monitor(shared, i, num, die_time) == END)
			break ;
		if (eatcnt_monitor(shared, num) == END)
			break ;
		i++;
		usleep(1000);
	}
	//printf("monitoring() 종료. 왜 쉬는지는 모르겠군요.\n");
	return (NULL);
}
