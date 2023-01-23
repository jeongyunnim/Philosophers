/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/23 20:56:29 by jeseo            ###   ########.fr       */
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

int	init_conditions(char *argv[], t_philo_conditions *conditions)
{
	char	option;

	option = 0;
	memset(conditions, 0, sizeof(conditions));
	conditions->philo_number = ft_atoi(argv[1]);
	conditions->time_to_die = ft_atoi(argv[2]);
	conditions->time_to_eat = ft_atoi(argv[3]);
	conditions->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5] != NULL)
	{
		option = 1;
		conditions->must_eat = ft_atoi(argv[5]);
	}
	if (conditions->philo_number <= 1 || conditions->time_to_die <= 0 || \
		conditions->time_to_eat <= 0 || conditions->time_to_sleep <= 0)
	{
		return (ERROR);
	}
	if (option == 1)
	{
		if (conditions->must_eat <= 0)
			return (ERROR);
	}
	return (0);
}

int	parse_arguments(char *argv[], t_philo_conditions *conditions)
{
	if (argument_num_check(argv) != 0)
	{
		write(1, "Error\nARGUMENT ISN'T NUMBER\n", 28);
		return (ERROR);
	}
	if (init_conditions(argv, conditions) != 0)
	{
		write(1, "Error\nINVALID ARGUMENT\n", 23);
		return (ERROR);
	}
	return (0);
}

/*
	if (if he can pick up the right fork)
        lock->fork[cnt - 1] = cnt;
    if (if he can pick up the left fork)
    {
        if (cnt != 1)
            lock->fork[cnt - 2] = cnt;
        else
            lock->fork[lock->total - 1] = cnt;
    }
*/

void	eating_spagetti(t_lock *lock, int cnt)
{
	int	left_fork;
	int	right_fork;

	left_fork = cnt - 1;
	if (cnt == 1)
		right_fork = lock->conditions->philo_number - 1;
	else
		right_fork = cnt - 2;
	printf("%dth philosopher is thinking(actually wating...)\n", cnt);
	pthread_mutex_lock(&lock->fork[left_fork]);
	printf("%dth philosopher picked up left fork\n", cnt);
	pthread_mutex_lock(&lock->fork[right_fork]);
	printf("%dth philosopher picked up right fork\n", cnt);
	printf("%dth philosopher is eating\n", cnt);
	pthread_mutex_unlock(&lock->fork[left_fork]);
	pthread_mutex_unlock(&lock->fork[right_fork]);
}

void	sleeping(t_lock *lock, int cnt)
{
	printf("%dth philosopher is sleeping\n", cnt);
	usleep(lock->conditions->time_to_sleep);
}

void	*philosopher_do_something(void *fork)
{
	static int  cnt;
	t_lock      *lock;

	cnt++;
	lock = (t_lock *)fork;
	printf("Hi, i'm philosopher %d! I have %p\n", cnt, lock);
	while (1)
	{
		eating_spagetti(lock, cnt);
		sleeping(lock, cnt);
	}
	return (NULL);
}

int	generate_philo(t_philo_conditions *conditions, pthread_t **philo)
{
	pthread_t		philosophers[conditions->philo_number];
	t_lock			locks;
	pthread_mutex_t	fork[conditions->philo_number];
	int				i;

	*philo = philosophers;
	memset(fork, 0, sizeof(fork));
	memset(&locks, 0, sizeof(locks));
	locks.conditions = conditions;

	i = 0;
	while (i < conditions->philo_number)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
	locks.fork = fork;
	i = 0;
	while (i < conditions->philo_number)
	{
		pthread_create(&philosophers[i], NULL, philosopher_do_something, &locks);
		i++;
	}
	i = 0;
	while (i < conditions->philo_number)
	{
		pthread_join(philosophers[i], NULL);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	pthread_t	*philos;
	t_philo_conditions	conditions;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nARGUMENT COUNT ERROR\n", 27);
		return (ERROR);
	}
	if (parse_arguments(argv, &conditions) != 0)
	{
		return (ERROR);
	}
	generate_philo(&conditions, &philos);
	return (0);
}
