/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 21:22:38 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	end_check(t_philo *shared)
{
	char	flag;

	pthread_mutex_lock(&shared->mutexes[END_M]);
	flag = shared->end_flag;
	pthread_mutex_unlock(&shared->mutexes[END_M]);
	return (flag);
}

int	main(int argc, char *argv[])
{
	t_philo			philo_share;
	t_conditions	conditions;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nArgument count error\n", 27);
		return (ERROR);
	}
	if (parse_arguments(argv, &conditions) != 0)
		return (ERROR);
	init_shared_mem(&philo_share, &conditions);
	if (generate_philo(&philo_share) == ERROR)
	{
		pthread_mutex_lock(&philo_share.mutexes[END_M]);
		philo_share.end_flag = END;
		pthread_mutex_unlock(&philo_share.mutexes[END_M]);
		wait_philos(&philo_share);
		free_structure(&philo_share);
		write(2, "Error\nPthread_create error\n", 27);
		return (ERROR);
	}
	thread_monitoring(&philo_share);
	wait_philos(&philo_share);
	free_structure(&philo_share);
	return (0);
}
