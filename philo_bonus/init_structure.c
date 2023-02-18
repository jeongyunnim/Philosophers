/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:46:51 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:45:49 by jeseo            ###   ########.fr       */
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
	shared->forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRWXU, num);
	if (shared->forks == SEM_FAILED)
		return (ERROR);
	shared->print_mutex = sem_open(PRT_NAME, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (shared->print_mutex == SEM_FAILED)
		return (ERROR);
	shared->event_mutex = sem_open(EVT_NAME, O_CREAT | O_EXCL, S_IRWXU, 1);
	if (shared->event_mutex == SEM_FAILED)
		return (ERROR);
	return (0);
}

void	free_structure(t_philo *shared)
{
	sem_close(shared->forks);
	sem_close(shared->event_mutex);
	sem_close(shared->print_mutex);
	sem_unlink(SEM_NAME);
	sem_unlink(EVT_NAME);
	sem_unlink(PRT_NAME);
	memset(shared, 0, sizeof(*shared));
}
