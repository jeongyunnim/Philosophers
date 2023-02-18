/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:46:51 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 16:47:00 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	init_shared_mem(t_philo *shared, t_conditions *conditions)
{
	int	num;

	sem_unlink(SEM_NAME);
	sem_unlink(EVT_NAME);
	sem_unlink(PRT_NAME);
	memset(shared, 0, sizeof(*shared));
	num = conditions->philo_number;
	shared->conditions = conditions;
	shared->forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, num);
	if (shared->forks == SEM_FAILED) 
	{
   		perror("sem_open(forks)");
   		exit(EXIT_FAILURE);
	}
	shared->print_mutex = sem_open(PRT_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	if (shared->print_mutex == SEM_FAILED) 
	{
   		perror("sem_open(print)");
   		exit(EXIT_FAILURE);
	}
	shared->event_mutex = sem_open(EVT_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
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
	sem_close(shared->print_mutex);
	// printf("unlink 할께\n");
	sem_unlink(SEM_NAME);
	sem_unlink(EVT_NAME);
	sem_unlink(PRT_NAME);
	memset(shared, 0, sizeof(*shared));
}
