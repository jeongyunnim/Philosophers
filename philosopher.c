/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/27 21:14:52 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	configure_time_stamp(t_philo *lock)
{
	long    passed_sec;
    long    passed_usec;
	long    present_point;

	gettimeofday(&lock->tv, NULL);
	passed_sec = lock->tv.tv_sec - lock->start_point.tv_sec;
	passed_usec = lock->tv.tv_usec - lock->start_point.tv_usec;
    present_point = passed_sec * 1000 + passed_usec / 1000;
    lock->time_stamp = present_point;
}

void	print_status(t_philo *lock, int num, char status)
{
//	configure_time_stamp(lock);
	pthread_mutex_lock(&lock->mutex[TIMEVAL_M]);
	if (status == EAT)
	{
		pthread_mutex_lock(&lock->mutex[LASTEAT_M]);
		lock->last_eat[num - 1] = lock->time_stamp;
        pthread_mutex_unlock(&lock->mutex[LASTEAT_M]);
		printf("%ld %d is eating\n", lock->time_stamp, num);
	}
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", lock->time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", lock->time_stamp, num);
	else if (status == DEAD)
		printf("%ld %d died\n", lock->time_stamp, num);
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", lock->time_stamp, num);
	pthread_mutex_unlock(&lock->mutex[TIMEVAL_M]);
}

void	pick_up_forks(t_philo *lock, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&lock->fork[right_fork]);
	pthread_mutex_lock(&lock->fork[left_fork]);
	print_status(lock, num, FORK);
}

//void	dead_check(t_philo *lock, int num)
//{
//	long	passed_sec;
//	int		passed_usec;

//	gettimeofday(&lock->tv, NULL);
//	passed_sec = lock->tv.tv_sec - lock->start_point.tv_sec;
//	passed_usec = lock->tv.tv_usec - lock->start_point.tv_usec;
//}

void	eating_spagetti(t_philo *lock, int num, int left_fork, int right_fork)
{
	int	time_to_eat;

	time_to_eat = lock->conditions->time_to_eat;
	print_status(lock, num, THINK);
	pick_up_forks(lock, num, left_fork, right_fork);
	print_status(lock, num, EAT);
	usleep(time_to_eat * 1000);
	pthread_mutex_unlock(&lock->fork[left_fork]);
	pthread_mutex_unlock(&lock->fork[right_fork]);
}

void	sleeping(t_philo *lock, int num)
{
	print_status(lock, num, SLEEP);
	usleep(lock->conditions->time_to_sleep * 1000);
	exit(EXIT_SUCCESS);
}

void	*philosopher_do_something(void *fork)
{
	t_philo	*lock;
	int		num;
	int		left_fork;
	int		right_fork;

	lock = (t_philo *)fork;
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

int survive_check(t_philo *lock)
{
	unsigned int	i;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&lock->mutex[TIMEVAL_M]);
		configure_time_stamp(lock);
		pthread_mutex_lock(&lock->mutex[LASTEAT_M]);
		pthread_mutex_lock(&lock->mutex[DIEFLAG_M]);
		if (lock->time_stamp - lock->last_eat[i % lock->index] < lock->conditions->time_to_die)
		{
			printf("현재시간 - 마지막 먹은 시간 %ld\n", lock->time_stamp - lock->last_eat[i % lock->index]);
			lock->die_flags[i] = DEAD;
		}
		pthread_mutex_unlock(&lock->mutex[DIEFLAG_M]);
		pthread_mutex_unlock(&lock->mutex[LASTEAT_M]);
		pthread_mutex_unlock(&lock->mutex[TIMEVAL_M]);
		i++;
	}
}

int	generate_philo(t_philo_conditions *conditions, pthread_t **philo, t_philo *shared)
{
    pthread_t		philosophers[conditions->philo_number];
    int				i;

    *philo = philosophers;
    i = 0;
    while (i < conditions->philo_number)
	{
        shared->index++;
        pthread_create(&philosophers[i], NULL, philosopher_do_something, &shared);
        pthread_detach(philosophers[i]);
        i++;
    }
    survive_check(shared);
    return (0);
}

void    init_philo(t_philo *philo_shared, int cnt)
{
    pthread_mutex_t	fork[cnt];
    long    *last_eat;
    char    *die_flags;
    int     i;

	i = 0;
    gettimeofday(&philo_shared->start_point, NULL);
	while (i < cnt)
	{
		pthread_mutex_init(&fork[i], NULL);
		i++;
	}
    pthread_mutex_init(&philo_shared->mutex[0], NULL);
    pthread_mutex_init(&philo_shared->mutex[1], NULL);
    pthread_mutex_init(&philo_shared->mutex[2], NULL);
    philo_shared->fork = fork;
    last_eat = (long *)malloc(sizeof(long) * cnt);
    die_flags = (char *)malloc(sizeof(char) * cnt);
    memset(last_eat, 0, sizeof(long) * cnt);
    memset(die_flags, 0, sizeof(char) * cnt);
    philo_shared->last_eat = last_eat;
    philo_shared->die_flags = die_flags;
}

int	main(int argc, char *argv[])
{
	pthread_t           *philos;
    t_philo             philo_share;
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
    init_philo(&philo_share, conditions.philo_number);
	generate_philo(&conditions, &philos, &philo_share);
	return (0);
}
