/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/30 15:45:24 by jeseo            ###   ########.fr       */
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
	pthread_mutex_lock(&lock->mutex[TIMEVAL_M]);
	if (status == EAT)
	{
		pthread_mutex_lock(&lock->mutex[LASTEAT_M]);
		lock->last_eat[num] = lock->time_stamp;
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
	else if (status == 5)
		printf("%ld %d has put a fork\n", lock->time_stamp, num);
	pthread_mutex_unlock(&lock->mutex[TIMEVAL_M]);
}

void	pick_up_forks(t_philo *lock, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&lock->fork[right_fork]);
	pthread_mutex_lock(&lock->fork[left_fork]);
	print_status(lock, num, FORK);
}

void	eating_spagetti(t_philo *lock, int num, int left_fork, int right_fork)
{
	int	time_to_eat;

	time_to_eat = lock->conditions->time_to_eat;
	pick_up_forks(lock, num, left_fork, right_fork);
	print_status(lock, num, EAT);
	usleep(time_to_eat * 1000);
	pthread_mutex_unlock(&lock->fork[left_fork]);
	pthread_mutex_unlock(&lock->fork[right_fork]);
	print_status(lock, num, 5);
}

void	sleeping(t_philo *lock, int num)
{
	print_status(lock, num, SLEEP);
	usleep(lock->conditions->time_to_sleep * 1000);
}

void	*philosopher_do_something(void *fork)
{
	t_philo	*lock;
	int		num;
	int		left_fork;
	int		right_fork;

	lock = (t_philo *)fork;
    pthread_mutex_lock(&lock->mutex[INDEXFLAG_M]);
    num = lock->index;
    pthread_mutex_unlock(&lock->mutex[INDEXFLAG_M]);
	right_fork = num - 1;
	if (num == 1)
		left_fork = lock->conditions->philo_number - 1;
	else
		left_fork = num - 2;
	while (1)
	{
		print_status(lock, num, THINK);
		eating_spagetti(lock, num, left_fork, right_fork);
 		sleeping(lock, num);
	}
	return (NULL);
}

int survive_check(t_philo *lock)
{
	unsigned int	i;
    //long            last_eat_val;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&lock->mutex[TIMEVAL_M]);
		configure_time_stamp(lock);
		pthread_mutex_lock(&lock->mutex[LASTEAT_M]);
		pthread_mutex_lock(&lock->mutex[DIEFLAG_M]);
		if (lock->time_stamp - lock->last_eat[i % lock->conditions->philo_number] < lock->conditions->time_to_die)
		{
			printf("%d가 마지막 먹은 시간 %ld\n", i % lock->index,  lock->time_stamp - lock->last_eat[i % lock->index]);
			lock->die_flags[i % lock->conditions->philo_number] = DEAD;
		}
		//else
		//{
		//	last_eat_val = lock->last_eat[i % lock->index];
		//	printf("??? %ld\n", last_eat_val);
		//}
		pthread_mutex_unlock(&lock->mutex[DIEFLAG_M]);
		pthread_mutex_unlock(&lock->mutex[LASTEAT_M]);
        pthread_mutex_unlock(&lock->mutex[TIMEVAL_M]);
		i++;
	}
}

int	generate_philo(t_philo_conditions *conditions, pthread_t **philo, t_philo *shared)
{
    pthread_t		philosophers[conditions->philo_number];
    long			last_eat[conditions->philo_number];
    char			die_flags[conditions->philo_number];
    int				i;

    *philo = philosophers;
    i = 0;
    memset(last_eat, 0, sizeof(last_eat));
    memset(die_flags, 0, sizeof(die_flags));
    shared->conditions = conditions;
    shared->last_eat = last_eat;
    shared->die_flags = die_flags;
    while (i < conditions->philo_number)
	{
        pthread_mutex_lock(&shared->mutex[INDEXFLAG_M]);
        shared->index += 1;
        pthread_mutex_unlock(&shared->mutex[INDEXFLAG_M]);
        pthread_create(&philosophers[i], NULL, philosopher_do_something, shared);
        pthread_detach(philosophers[i]);
        i++;
    }
    survive_check(shared);
    return (0);
}

void    init_philo(t_philo *philo_shared, int cnt)
{
    pthread_mutex_t	fork[cnt];
    int             i;

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
    pthread_mutex_init(&philo_shared->mutex[3], NULL);
    philo_shared->fork = fork;
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
    memset(&philo_share, 0, sizeof(philo_share));
    init_philo(&philo_share, conditions.philo_number);
	generate_philo(&conditions, &philos, &philo_share);
	return (0);
}
