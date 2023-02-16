/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:46:51 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/15 20:00:13 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	init_shared_mem(t_philo *shared, t_conditions *conditions)
{
	int	num;
	//int	status;

	sem_unlink(SEM_NAME);
	sem_unlink(MUT_NAME);
	memset(shared, 0, sizeof(*shared));
	num = conditions->philo_number;
	shared->conditions = conditions;
	shared->forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, num);
	if (shared->forks == SEM_FAILED) 
	{
   		perror("sem_open(forks)");
   		exit(EXIT_FAILURE);
	}
	shared->event_mutex = sem_open(MUT_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	if (shared->event_mutex == SEM_FAILED)
	{
		perror ("sem_open(event_mutex)");
		exit(EXIT_FAILURE);
	}
	return (0);
}

void	free_structure(t_philo *shared)
{
	sem_close(shared->forks);
	sem_close(shared->event_mutex);
	// printf("unlink 할께\n");
	sem_unlink(SEM_NAME);
	sem_unlink(MUT_NAME);
	memset(shared, 0, sizeof(*shared));
}
