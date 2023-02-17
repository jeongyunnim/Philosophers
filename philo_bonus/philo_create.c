/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/17 15:58:36 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	generate_philo(t_philo *shared)
{
	pid_t	*pid;

	shared->start = get_time();
	pid = (pid_t *)ft_calloc(shared->conditions->philo_number, sizeof(pid_t));
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
			return (ERROR);
		}
	}
}

void	philo_wait(void)
{
	pid_t	flag;

	while (1)
	{
		flag = waitpid(-1, NULL, WNOHANG);
	}
}

// void	philo_wait(t_philo *shared)
// {
// 	int	status;
// 	// int	num;

// 	// num = shared->conditions->philo_number;
// 	waitpid(-1, &status, 0);
// 	kill(0, SIGKILL);
// 	// while(i < num)
// 	// {
// 	// 	i++;
// 	// }
// }