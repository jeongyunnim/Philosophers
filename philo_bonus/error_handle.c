/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:04:04 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:45:36 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	fork_error(t_philo *shared, pid_t *pid)
{
	int	i;

	free_structure(shared);
	if (pid != NULL)
	{
		i = 0;
		while (i < shared->index)
		{
			kill(pid[i], SIGKILL);
			i++;
		}
		free(pid);
		write(2, "Error\nfork error\n", 19);
	}
	return (ERROR);
}

int	init_error(void)
{
	write(2, "Error\nInit Error\n", 17);
	return (ERROR);
}
