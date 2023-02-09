/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:46:51 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/09 16:48:22 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	destroy_mutex_free_array(pthread_mutex_t **arr, int i)
{
	while (--i != 0)
	{
		pthread_mutex_destroy(&(*arr[i]));
	}
	free(*arr);
}

pthread_mutex_t	*init_mutex_array(int num)
{
	pthread_mutex_t	*mutex_arr;
	int				i;

	i = 0;
	mutex_arr = (pthread_mutex_t *)ft_calloc(sizeof(pthread_mutex_t), num);
	if (mutex_arr == NULL)
		return (NULL);
	while (i < num)
	{
		if (pthread_mutex_init(&mutex_arr[i], NULL) != 0)
		{
			destroy_mutex_free_array(&mutex_arr, i);
			return (NULL);
		}
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

int	init_shared_mem(t_philo *philo_shared, t_conditions *conditions)
{
	int	num;

	memset(philo_shared, 0, sizeof(*philo_shared));
	num = conditions->philo_number;
	philo_shared->conditions = conditions;
	philo_shared->fork_mutex = init_mutex_array(num);
	philo_shared->mutexes = init_mutex_array(TOTAL_MUTEX);
	philo_shared->philos = (pthread_t *)ft_calloc(sizeof(pthread_t), num);
	if (philo_shared->philos == NULL)
		return (ERROR);
	philo_shared->last_eat = (long *)ft_calloc(sizeof(long), num);
	if (philo_shared->last_eat == NULL)
		return (ERROR);
	philo_shared->fork = (int *)ft_calloc(sizeof(int), num);
	if (philo_shared->fork == NULL)
		return (ERROR);
	philo_shared->eat_cnt = (int *)ft_calloc(sizeof(int), num);
	if (philo_shared->eat_cnt == NULL)
		return (ERROR);
	return (0);
}

void	free_structure(t_philo *shared)
{
	int	num;

	num = shared->conditions->philo_number;
	destroy_mutex_array(shared->fork_mutex, num);
	destroy_mutex_array(shared->mutexes, TOTAL_MUTEX);
	free(shared->fork_mutex);
	free(shared->mutexes);
	free(shared->philos);
	free(shared->last_eat);
	free(shared->fork);
	free(shared->eat_cnt);
	memset(shared, 0, sizeof(*shared));
}
