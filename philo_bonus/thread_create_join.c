/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_create_join.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 15:44:07 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	generate_philo(t_philo *shared)
{
	int		i;

	i = 0;
	shared->start = get_time();
	while (i < shared->conditions->philo_number)
	{
		if (pthread_create(&shared->philos[i], NULL, philo_do, shared) != 0)
			return (ERROR);
		i++;
	}
	return (0);
}

void	wait_philos(t_philo *shared)
{
	int	num;
	int	i;

	num = shared->conditions->philo_number;
	i = 0;
	while (i < num)
	{
		pthread_join(shared->philos[i], NULL);
		i++;
	}
}
