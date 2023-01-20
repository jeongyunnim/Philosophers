/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/20 21:23:10 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	parse_arguments(char *argv[], t_philo_conditons *conditions)
{
	return (0);
}

void	*eat(void *arg)
{
	return (NULL);
}

int	main(int argc, char *argv[])
{
	pthread_t			philo;
	t_philo_conditons	conditions;
	int					result;

	if (argc != 4 && argc != 5)
	{
		write(2, "Error\nARGUMENT COUNT ERROR\n", 27);
		return (ERROR);
	}
	parse_arguments(argv, &conditions);
	memset(conditions, 0, sizeof(philo_conditions));
	result = pthread_create(&philo, NULL, eat, NULL);
	return (0);
}
