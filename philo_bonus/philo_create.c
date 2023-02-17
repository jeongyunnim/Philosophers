/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/17 20:43:03 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

pid_t	*generate_philo(t_philo *shared)
{
	pid_t	*pid;

	shared->start = get_time();
	pid = (pid_t *)ft_calloc(shared->conditions->philo_number, sizeof(pid_t));
	//ㄴㅓㄹ가드
	while (shared->index < shared->conditions->philo_number)
	{
		shared->index++;
		pid[shared->index] = fork();
		if (pid[shared->index] == 0)
		{
			break ;
		}
		else if (pid[shared->index] < 0)
		{
			write(2, "Error\nfork() error\n", 19);
			return (NULL);
		}
	}
	return (pid);
}

void	philo_wait(t_philo *shared, pid_t *childs)
{
	pid_t	pid;
	int		num;
	int		i;

	i = 0;
	num = shared->conditions->philo_number;
	while (1)
	{
		pid = waitpid(-1, NULL, WNOHANG);
		if (pid != 0)
		{
			printf("R U N%d\n", pid);
			while (i < num)
			{
				if (pid != childs[i])
					kill(childs[i], SIGKILL);
				i++;
			}
		}
	}
}
