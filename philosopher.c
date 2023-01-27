/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/27 14:12:24 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

long	configure_time_stamp(t_lock *lock)
{
	long	time_stamp;
	long	passed_sec;
	int		passed_usec;

	gettimeofday(&lock->tv, NULL);
	passed_sec = lock->tv.tv_sec - lock->start_point.tv_sec;
	passed_usec = lock->tv.tv_usec - lock->start_point.tv_usec;
	time_stamp = passed_sec * 1000 + passed_usec / 1000;
	return (time_stamp);
}

void	print_status(t_lock *lock, int num, char status)
{
	long	time_stamp;

	pthread_mutex_lock(&lock->tv_mutex);
	time_stamp = configure_time_stamp(lock);
	pthread_mutex_unlock(&lock->tv_mutex);
	if (status == EAT)
	{
		lock->last_eat[num - 1] = time_stamp;
		printf("%ld %d is eating\n", time_stamp, num);
	}
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", time_stamp, num);
	else if (status == DEAD)
		printf("%ld %d died\n", time_stamp, num);
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", time_stamp, num);
}

void	pick_up_forks(t_lock *lock, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&lock->fork[right_fork]);
	pthread_mutex_lock(&lock->fork[left_fork]);
	print_status(lock, num, FORK);
}

//void	dead_check(t_lock *lock, int num)
//{
//	long	passed_sec;
//	int		passed_usec;

//	gettimeofday(&lock->tv, NULL);
//	passed_sec = lock->tv.tv_sec - lock->start_point.tv_sec;
//	passed_usec = lock->tv.tv_usec - lock->start_point.tv_usec;
//}

void	eating_spagetti(t_lock *lock, int num, int left_fork, int right_fork)
{
	print_status(lock, num, THINK);
	pick_up_forks(lock, num, left_fork, right_fork);
	print_status(lock, num, EAT);
	usleep(lock->conditions->time_to_eat * 1000);
	pthread_mutex_unlock(&lock->fork[left_fork]);
	pthread_mutex_unlock(&lock->fork[right_fork]);
}

void	sleeping(t_lock *lock, int num)
{
	print_status(lock, num, SLEEP);
	usleep(lock->conditions->time_to_sleep * 1000);
}

void	*philosopher_do_something(void *fork)
{
	t_lock	*lock;
	int		num;
	int		left_fork;
	int		right_fork;

	lock = (t_lock *)fork;
	num = lock->index;
	right_fork = num - 1;
	if (num == 1)
		left_fork = lock->conditions->philo_number - 1;
	else
		left_fork = num - 2;
	while (1)
	{
		eating_spagetti(lock, num, left_fork, right_fork);
 		sleeping(lock, num);
	}
	return (NULL);
}

int survive_check(t_lock *lock)
{
	long			time_stamp;
	unsigned int	i;

	pthread_mutex_lock(&lock->tv_mutex);
	time_stamp = configure_time_stamp(lock);
	pthread_mutex_unlock(&lock->tv_mutex);
	i = 0;
	while (1)
	{
		if (time_stamp - lock->last_eat[i % lock->index] < lock->conditions->time_to_die)
			lock->last_eat[i] = DEAD;
		i++;
	}
}

int	generate_philo(t_philo_conditions *conditions, pthread_t **philo)
{
	pthread_t		philosophers[conditions->philo_number];
	pthread_mutex_t	fork[conditions->philo_number];
	t_lock			locks;
//	int				last_eat[conditions->philo_number];
	int				i;

	*philo = philosophers;
	memset(fork, 0, sizeof(fork));
	memset(&locks, 0, sizeof(locks));
	locks.conditions = conditions;
	pthread_mutex_init(&locks.tv_mutex, NULL);
	i = 0;
	while (i < conditions->philo_number)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
	locks.fork = fork;
	gettimeofday(&locks.start_point, NULL);
	i = 0;
	while (i < conditions->philo_number)
	{
		locks.index++;
		pthread_create(&philosophers[i], NULL, philosopher_do_something, &locks);
		pthread_detach(philosophers[i]);
		i++;
	}
	survive_check(&locks);
	return (0);
}

int	main(int argc, char *argv[])
{
	pthread_t           *philos;
	t_philo_conditions  conditions;

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
