/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 17:29:53 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

pid_t	*generate_philo(t_philo *shared)
{
	pid_t	*pid;

	shared->start = get_time();
	pid = (pid_t *)ft_calloc(shared->conditions->philo_number, sizeof(pid_t));
	//null가드
	while (shared->index < shared->conditions->philo_number)
	{
		pid[shared->index] = fork();
		if (pid[shared->index] == 0)
		{
			free(pid);
			break ;
		}
		else if (pid[shared->index] < 0)
		{
			write(2, "Error\nfork() error\n", 19);
			return (NULL);
		}
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
	printf("필로 기다리자\n");
	while (1)
	{
		pid = waitpid(-1, NULL, WNOHANG);
		//printf("pid: %d\n", pid);
		if (pid != 0 && pid != -1)
		{
			i = 0;
			while (i < num)
			{
				if (pid != childs[i])
					kill(childs[i], SIGKILL);
				i++;
			}
			return ;
		}
		else if (pid == -1)
		{
			perror("waitpid error");
		}
		usleep(256);
	}
}
