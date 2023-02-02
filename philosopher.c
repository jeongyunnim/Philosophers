/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/02 20:40:13 by jeseo            ###   ########.fr       */
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
	pthread_mutex_lock(&lock->struct_mutex); // 누가 사용 중이니?
	if (status == EAT)
	{
		lock->last_eat[num] = lock->time_stamp;
		printf("%ld %d is eating\n", lock->time_stamp, num);
	}
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", lock->time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", lock->time_stamp, num);
	else if (status == DEAD)
	{
		printf("%ld %d died\n", lock->time_stamp, num);
	}
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", lock->time_stamp, num);
	else if (status == 5)
		printf("%ld %d has put a fork\n", lock->time_stamp, num);
	pthread_mutex_unlock(&lock->struct_mutex);
}

void	pick_up_forks(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	shared->fork[right_fork] = 1;
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	shared->fork[left_fork] = 1;
	print_status(shared, num, FORK);
}

//void	split_usleep(int mili_sec)
//{
//	struct timeval	standard_point;
//	struct timeval	passed_ms;

//	gettimeofday(&standard_point, NULL);
//	while (mili_sec > 0)
//	{
//		gettimeofday(&passed_ms, NULL);

//	}
//}

void	eating_spagetti(t_philo *shared, int num, int left_fork, int right_fork)
{
	pick_up_forks(shared, num, left_fork, right_fork);
	
	print_status(shared, num, EAT);
	usleep(shared->conditions->time_to_eat * 1000);
	shared->fork[left_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[left_fork]);
	shared->fork[right_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[right_fork]);
	print_status(shared, num, 5);
}

void	sleeping(t_philo *lock, int num)
{
	print_status(lock, num, SLEEP);
	usleep(lock->conditions->time_to_sleep * 1000);
}

int	dead_check(t_philo *shared, int num)
{
	printf("%d 디짐?\n", num);
	pthread_mutex_lock(&shared->struct_mutex);
	if (shared->die_flags == DEAD)
	{
		printf("%d 응 디짐\n", num);
		pthread_mutex_unlock(&shared->struct_mutex);
		return (DEAD);
	}
	printf("%d 살아있다.\n", num);
	pthread_mutex_unlock(&shared->struct_mutex);
	return (0);
}

void	*philosopher_do_something(void *fork)
{
	t_philo	*lock;
	int		num;
	int		left_fork;
	int		right_fork;

	lock = (t_philo *)fork;
	pthread_mutex_lock(&lock->struct_mutex);
	num = lock->index;
	pthread_mutex_unlock(&lock->struct_mutex);
	right_fork = num - 1;
	if (num == 1)
		left_fork = lock->conditions->philo_number - 1;
	else
		left_fork = num - 2;
	while (1)
	{
		if (dead_check(lock, num) != DEAD)
		{
			eating_spagetti(lock, num, left_fork, right_fork);
			printf("%d 왜 안 자니\n", num);		
		}
		if (dead_check(lock, num) != DEAD)
		 	sleeping(lock, num);
		if (dead_check(lock, num) != DEAD)
			print_status(lock, num, THINK);
		else
		{
			print_status(lock, num, DEAD);
			return (NULL);
		}
	}
	return (NULL);
}

int survive_check(t_philo *shared)
{
	unsigned int	i;

	i = 0;
	while (1)
	{
		configure_time_stamp(shared);
		if (shared->time_stamp - shared->last_eat[i % shared->conditions->philo_number] < shared->conditions->time_to_die)
		{
			shared->die_flags = DEAD;
			print_status(shared, i % shared->conditions->philo_number, DEAD);
		}
		i++;
		usleep(100);
	}
}

int	generate_philo(t_conditions *conditions, pthread_t **philo, t_philo *shared)
{
	int				i;

    i = 0;

	*philo = (pthread_t *)calloc(sizeof(pthread_t), conditions->philo_number);
    shared->conditions = conditions;
	i = 0;
    while (i < conditions->philo_number)
	{
        pthread_mutex_lock(&shared->mutexes[INDEX_M]);
        shared->index += 1;1
        pthread_mutex_unlock(&shared->mutexes[INDEX_M]);
        pthread_create(philo[i], NULL, philosopher_do_something, shared);
        pthread_detach(*philo[i]);
		pthread_mutex_lock(&shared->mutexes[WAIT_M]);
        i++;
    }


    survive_check(shared);
    return (0);
}

void	init_mutex_array(pthread_mutex_t **mutex_arr, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_init(mutex_arr[i], NULL);
		i++;
	}
}

void    init_shared_mem(t_philo *philo_shared, t_conditions *conditions) // calloc 사용 중
{
	int	num;
    int	i;

	num = conditions->philo_number;
	philo_shared->conditions = conditions;
	philo_shared->fork_mutex = (pthread_mutex_t *)calloc(sizeof(pthread_mutex_t), num);
	init_mutex_array(philo_shared->fork_mutex, num);
	init_mutex_array(philo_shared->mutexes, 3);
	philo_shared->last_eat = (long *)calloc(sizeof(long), num);
	philo_shared->fork = (int *)calloc(sizeof(int), num);
}

int	main(int argc, char *argv[])
{
	pthread_t           *philos;
    t_philo             philo_share;
	t_conditions  		conditions;

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
    init_shared_mem(&philo_share, &philos, &conditions);
	generate_philo(&conditions, &philos, &philo_share);
	return (0);
}
