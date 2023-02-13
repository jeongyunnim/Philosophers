/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:29:37 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/13 17:16:49 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	dead_full_monitor(t_philo *shared, int num)
{
	int	minimum;
	int	must_eat;
	int	die_time;

	must_eat = shared->conditions->must_eat;
	die_time = shared->conditions->time_to_die;
	if (shared->last_eat + die_time < get_time() - shared->start)
		print_status(shared, num, DEAD);
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
	shared->end_flag = END;
	return (NULL);
}
