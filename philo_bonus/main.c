/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/15 19:39:51 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	main(int argc, char *argv[])
{
	t_philo			philo_share;
	t_conditions	conditions;
	pid_t			pid;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nArgument count error\n", 27);
		return (ERROR);
	}
	if (parse_arguments(argv, &conditions) != 0)
		return (ERROR);
	init_shared_mem(&philo_share, &conditions);
	pid = generate_philo(&philo_share);
	if (pid == -1)
		return (return_error(&philo_share));	
	else if (pid != 0)
	{
		philo_wait();
	}
	else
	{
		generate_thread(&philo_share);
		philo_do(&philo_share);
		free_structure(&philo_share);
		return (0);
	}
	free_structure(&philo_share);
	return (0);
}
