/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:30:38 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	main(int argc, char *argv[])
{
	t_philo			philo_share;
	t_conditions	conditions;
	pid_t			*pid;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nArgument count error\n", 27);
		return (ERROR);
	}
	if (parse_arguments(argv, &conditions) != 0)
		return (ERROR);
	if (init_shared_mem(&philo_share, &conditions) == ERROR)
		return (init_error());
	pid = generate_philo(&philo_share);
	if (pid == NULL)
		return (fork_error(&philo_share, pid));
	else if (philo_share.index == conditions.philo_number)
		parent_process_do(&philo_share, pid);
	else
		child_process_do(&philo_share);
	return (0);
}
