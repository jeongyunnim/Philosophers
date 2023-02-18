/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:47:42 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

pid_t	*generate_philo(t_philo *shared)
{
	pid_t	*pid;

	pid = (pid_t *)ft_calloc(shared->conditions->philo_number, sizeof(pid_t));
	if (pid == NULL)
	{
		write(2, "Error\nAllocate Error\n", 21);
		return (NULL);
	}
	shared->start = get_time();
	while (shared->index < shared->conditions->philo_number)
	{
		pid[shared->index] = fork();
		if (pid[shared->index] == 0)
			break ;
		else if (pid[shared->index] < 0)
			return (NULL);
		shared->index++;
	}
	return (pid);
}

void	philo_wait(t_philo *shared, pid_t *childs)
{
	pid_t	pid;
	int		num;
	int		i;

	num = shared->conditions->philo_number;
	while (1)
	{
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid == -1)
		{
			write(2, "Error\nwaitpid() Error\n", 22);
			return ;
		}
		else if (pid != 0)
		{
			i = -1;
			while (++i < num)
			{
				if (pid != childs[i])
					kill(childs[i], SIGKILL);
			}
			return ;
		}
		usleep(256);
	}
}
