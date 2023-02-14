/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structure.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 17:46:51 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/14 21:09:16 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	init_shared_mem(t_philo *shared, t_conditions *conditions)
{
	int	num;

	memset(shared, 0, sizeof(*shared));
	num = conditions->philo_number;
	shared->conditions = conditions;
	shared->forks = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, num);
	shared->evnet_mutex = sem_open("/mutex_sem", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	//그런데 각자 사용해야지 이걸 다 같이 사용하면 무슨 낭비야 이게 다른 애들은 다른 철학자가 먹었는지, 죽었는지 체크할 때 세마포어를 다 같이 걸어주는 것은 낭비인 것으로 보인다.
	if (shared->forks == SEM_FAILED)
		return (ERROR);
	return (0);
}

void	free_structure(t_philo *shared)
{
	int	num;

	num = shared->conditions->philo_number;
	memset(shared, 0, sizeof(*shared));
}
