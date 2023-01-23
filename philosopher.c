/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/21 19:58:24 by jeseo            ###   ########.fr       */
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

void    check_his_hands(t_lock *lock, int cnt)
{
    if (cnt != 1)
    {
        if (lock->fork[cnt - 2] == cnt && lock->fork[cnt - 1] == cnt);
            eat(cnt);
    }
    else
    {
        if (lock->fork[lock->total - 1] == cnt && lock->fork[cnt - 1] == cnt);
            eat(cnt);
    }
}

void	*philosopher_do_something(void *fork)
{
	static int  cnt;
    t_lock      *lock;

    cnt++;
    lock = (t_lock *)fork;
    if (if he can pick up the right fork)
        lock->fork[cnt - 1] = cnt;
    if (if he can pick up the left fork)
    {
        if (cnt != 1)
            lock->fork[cnt - 2] = cnt;
        else
            lock->fork[lock->total - 1] = cnt;
    }
    check_his_hands(lock, cnt);
	printf("Hi, i'm philosopher %d! I have %p\n", cnt, lock);
	return (NULL);
}

int	generate_philo(t_philo_conditions conditions, pthread_t **philo)
{
    pthread_t   philosophers[conditions.philo_number];
    t_lock      mutexes;
    int         fork[conditions.philo_number];
	int	        i;

	i = 0;
    *philo = philosophers;
    memset(fork, 0, sizeof(fork));
    memset(&mutexes, 0, sizeof(mutexes));
    mutexes.fork = fork;
    mutexes.total = conditions.philo_number;
	while (i < conditions.philo_number)
	{
		pthread_create(&philosophers[i], NULL, philosopher_do_something, &mutexes);
		i++;
	}
    i = 0;
    while (i < conditions.philo_number)
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
	generate_philo(conditions, &philos);
	return (0);
}
