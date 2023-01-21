/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/21 17:08:06 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_isnum(char c)
{
	if ('0' <= c && c <= '9')
	{
		return (0);
	}
	return (-1);
}

int	argument_num_check(char *argv[])
{
	int	i;
	int	j;

	i = 1;
	if (argv[i] != NULL)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (ft_isnum(argv[i][j]) != 0)
				return (ERROR);
			j++;
		}
		i++;
	}
	return (0);
}

int	init_conditions(char *argv[], t_philo_conditons *conditions)
{
	char	option;

	option = 0;
	conditions->philo_number = ft_atoi(argv[1]);
	conditions->time_to_die = ft_atoi(argv[2]);
	conditions->time_to_eat = ft_atoi(argv[3]);
	conditions->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
	{
		option = 1;
		conditions->must_eat = ft_atoi(argv[5]);
	}
	printf("왜?");
	if (conditions->philo_number <= 1 || conditions->time_to_die <= 0 || \
		conditions->time_to_eat <= 0 || conditions->time_to_sleep <= 0)
		return (ERROR);
	if (option == 1)
	{
		if (conditions->must_eat <= 0)
			return (ERROR);
	}
	return (0);
}

int	parse_arguments(char *argv[], t_philo_conditons *conditions)
{
	if (argument_num_check(argv) != 0)
	{
		write(1, "Error\nARGUMENT ISN'T NUMBER\n", 28);
		return (ERROR);
	}
	if (init_conditions(argv, conditions) != 0)
	{
		write(1, "Error\nINVALID ALGUMENT\n", 23);
		return (ERROR);
	}
	return (0);
}

void	*philo_init(void *i)
{
	printf("philospher%d 등장\n", (int)i);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	pthread_t			philo;
	t_philo_conditons	conditions;
	pthread_attr_t		attr;
	int					i;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nARGUMENT COUNT ERROR\n", 27);
		return (ERROR);
	}
	parse_arguments(argv, &conditions);
	pthread_attr_init(&attr);
	i = 1;
	if (pthread_create(&philo, NULL, philo_init, &index) != 0)
		return (ERROR);
	return (0);
}
