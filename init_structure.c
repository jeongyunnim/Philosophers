/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:46:51 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 19:33:30 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

pthread_mutex_t	*init_mutex_array(int num)
{
	pthread_mutex_t	*mutex_arr;
	int				i;

	i = 0;
	mutex_arr = (pthread_mutex_t *)calloc(sizeof(pthread_mutex_t), num);
	while (i < num)
	{
		pthread_mutex_init(&mutex_arr[i], NULL);
		i++;
	}
	return (mutex_arr);
}

void	destroy_mutex_array(pthread_mutex_t *mutex_arr, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&mutex_arr[i]);
		i++;
	}
}

void	init_shared_mem(t_philo *philo_shared, t_conditions *conditions)
{
	int	num;

	memset(philo_shared, 0, sizeof(*philo_shared));
	num = conditions->philo_number;
	philo_shared->conditions = conditions;
	philo_shared->fork_mutex = init_mutex_array(num);
	philo_shared->mutexes = init_mutex_array(TOTAL_MUTEX);
	philo_shared->philos = (pthread_t *)calloc(sizeof(pthread_t), num);
	philo_shared->last_eat = (long *)calloc(sizeof(long), num);
	philo_shared->fork = (int *)calloc(sizeof(int), num);
	philo_shared->eat_cnt = (int *)calloc(sizeof(int), num);
}

void	free_structure(t_philo *shared)
{
	int	num;

	num = shared->conditions->philo_number;
	destroy_mutex_array(shared->fork_mutex, num);
	destroy_mutex_array(shared->mutexes, TOTAL_MUTEX);
	free(shared->philos);
	free(shared->last_eat);
	free(shared->fork);
	free(shared->eat_cnt);
	memset(shared, 0, sizeof(*shared));
}
