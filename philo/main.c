/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 23:23:40 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	main(int argc, char *argv[])
{
	t_philo			philo_share;
	t_conditions	conditions;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nArgument count error\n", 27);
		return (ERROR);
	}
	if (parse_arguments(argv, &conditions) != 0)
		return (ERROR);
	init_shared_mem(&philo_share, &conditions);
	if (generate_philo(&philo_share) == ERROR)
		return (return_error(&philo_share));
	thread_monitoring(&philo_share);
	wait_philos(&philo_share);
	free_structure(&philo_share);
	return (0);
}
