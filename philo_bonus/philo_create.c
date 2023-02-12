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
	pid_t	pid;
	pid_t	child_pid;
	int		status;
	int		i;

	i = 0;
	shared->start = get_time();
	while (i < shared->conditions->philo_number)
	{
		pid = fork();
		if (pid == 0)
		{
			printf("Child process %d\n", i + 1);
			break ;
		}
		else if (pid < 0)
		{
			perror("Failed to fork\n");
			return (ERROR);
		}
		i++;
	}
	return (pid);
}
