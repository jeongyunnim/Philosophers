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
	pid_t	*pid_arr;
	pid_t	child_pid;
	int		status;
	int		i;

	i = 0;
	shared->start = get_time();
	pid_arr = (pid_t *)ft_calloc(sizeof(pid_t), shared->conditions->philo_number);
	while (i < shared->conditions->philo_number)
	{
		pid_arr[i] = fork();
		if (pid_arr[i] == 0)
		{
			printf("Child process %d\n", i + 1);
			break ;
		}
		else if (pid_arr[i] < 0)
		{
			perror("Failed to fork\n");
			return (ERROR);
		}
		i++;
	}
	i = 0;
	if (pid_arr[i] > 0)
	{
		while (i < shared->conditions->philo_number)
		{
			child_pid = waitpid(pid_arr[i], &status, 0);
			printf("parent pid: %d child process %d completed\n", pid_arr[i], child_pid);
			i++; 
		}
	}
	return (0);
}
