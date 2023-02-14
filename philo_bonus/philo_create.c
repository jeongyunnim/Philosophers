/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/14 20:18:11 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	generate_philo(t_philo *shared)
{
	pid_t	pid;
	pid_t	child_pid;
	int		status;

	shared->start = get_time();
	pid = -1;
	while (shared->index < shared->conditions->philo_number)
	{
		pid = fork();
		if (pid == 0)
		{
			printf("Child process %d\n", getpid());
			break ;
		}
		else if (pid < 0)
		{
			perror("Failed to fork\n");
			return (ERROR);
		}
		shared->index++;
	}
	return (pid);
}
