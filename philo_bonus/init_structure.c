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
#define	SEM_NAME "/forks"

int	init_shared_mem(t_philo *shared, t_conditions *conditions)
{
	int	num;
	//int	status;

	sem_unlink(SEM_NAME);
	memset(shared, 0, sizeof(*shared));
	num = conditions->philo_number;
	shared->conditions = conditions;
	shared->forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, num);
	if (shared->forks == SEM_FAILED) 
	{
   		perror("sem_open");
   		exit(EXIT_FAILURE);
	}
	//status = sem_wait(shared->forks);
	//if (status == -1)
	//{
	//    perror("sem_wait");
	//    exit(EXIT_FAILURE);
	//}
	return (0);
}
	//shared->evnet_mutex = sem_open("/mutex_sem", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

void	free_structure(t_philo *shared)
{
	sem_close(shared->forks);
	sem_close(shared->evnet_mutex);
	// printf("unlink 할께\n");
	sem_unlink("/mysem");
	memset(shared, 0, sizeof(*shared));
}
