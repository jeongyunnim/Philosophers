/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 13:21:12 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/15 19:33:18 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	generate_philo(t_philo *shared)
{
	pid_t	pid;

	pid = -1;
	shared->start = get_time();
	while (shared->index < shared->conditions->philo_number)
	{
		shared->index++;
		pid = fork();
		if (pid == 0)
		{
			break ;
		}
		else if (pid < 0)
		{
			write(2, "Error\nfork() error\n", 19);
			return (ERROR);
		}
	}
	return (pid);
}

void	philo_wait(void)
{
	int	status;
	// int	num;

	// num = shared->conditions->philo_number;
	waitpid(-1, &status, 0);
	kill(0, SIGKILL);
	// while(i < num)
	// {
	// 	i++;
	// }
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