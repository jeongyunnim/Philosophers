/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/23 21:39:22 by jeseo            ###   ########.fr       */
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
	memset(conditions, 0, sizeof(*conditions));
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

void	pick_up_forks(t_lock *lock, int cnt, int left_fork, int right_fork)
{
	if (cnt % 2 == 1)
	{
		pthread_mutex_lock(&lock->fork[left_fork]);
		printf("%dth philosopher picked up left fork\n", cnt);
		pthread_mutex_lock(&lock->fork[right_fork]);
		printf("%dth philosopher picked up right fork\n", cnt);
	}
	else
	{
		pthread_mutex_lock(&lock->fork[right_fork]);
		printf("%dth philosopher picked up right fork\n", cnt);
		pthread_mutex_lock(&lock->fork[left_fork]);
		printf("%dth philosopher picked up left fork\n", cnt);
	}	
}

void	eating_spagetti(t_lock *lock, int num)
{
	int	left_fork;
	int	right_fork;

	right_fork = num - 2;
	if (num == 1)
		left_fork = lock->conditions->philo_number - 1;
	else
		left_fork = num - 1;
	printf("%dth philosopher is thinking(actually waiting...)\n", num);
	pick_up_forks(lock, num, left_fork, right_fork);
	printf("%dth philosopher is eating\n", num);
	usleep(lock->conditions->time_to_eat);
	pthread_mutex_unlock(&lock->fork[left_fork]);
	pthread_mutex_unlock(&lock->fork[right_fork]);
}

void	sleeping(t_lock *lock, int num)
{
	printf("%dth philosopher is sleeping\n", num);
	usleep(lock->conditions->time_to_sleep);
}

void	*philosopher_do_something(void *fork)
{
	static int	cnt;
    int         num;
	t_lock		*lock;

	cnt++;
	lock = (t_lock *)fork;
    num = cnt;
	printf("Hi, i'm philosopher %d!\n", num);
	while (1)
	{
		eating_spagetti(lock, num);
		sleeping(lock, num);
	}
	return (NULL);
}

int	generate_philo(t_philo_conditions *conditions, pthread_t **philo)
{
	pthread_t		philosophers[conditions->philo_number];
	pthread_mutex_t	fork[conditions->philo_number];
	t_lock			locks;
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
        pthread_detach(philosophers[i]);
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
