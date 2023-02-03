/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/03 19:01:09 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	dead_check(t_philo *shared)
{
	pthread_mutex_lock(&shared->mutexes[DIE_M]);
	if (shared->die_flags == DEAD)
	{
		pthread_mutex_unlock(&shared->mutexes[DIE_M]);
		return (DEAD);
	}
	pthread_mutex_unlock(&shared->mutexes[DIE_M]);
	return (0);
}

void	configure_time_stamp(t_philo *shared)
{
	long    passed_sec;
    long    passed_usec;

	pthread_mutex_lock(&shared->mutexes[TIME_M]);
	gettimeofday(&shared->tv, NULL);
	passed_sec = shared->tv.tv_sec - shared->start_point.tv_sec;
	passed_usec = shared->tv.tv_usec - shared->start_point.tv_usec;
    shared->time_stamp = passed_sec * 1000 + passed_usec / 1000;
	pthread_mutex_unlock(&shared->mutexes[TIME_M]);
}

void	print_status(t_philo *shared, int num, char status)
{
	pthread_mutex_lock(&shared->mutexes[TIME_M]);
	if (status == EAT)
	{
		pthread_mutex_lock(&shared->mutexes[LASTEAT_M]);
		shared->last_eat[num] = shared->time_stamp;
		pthread_mutex_unlock(&shared->mutexes[LASTEAT_M]);
		printf("%ld %d is eating\n", shared->time_stamp, num);
	}
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", shared->time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", shared->time_stamp, num);
	else if (status == DEAD)
	{
		printf("%ld %d died\n", shared->time_stamp, num);
	}
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", shared->time_stamp, num);
	pthread_mutex_unlock(&shared->mutexes[TIME_M]);
}

void	pick_up_forks(t_philo *shared, int num, int left_fork, int right_fork)
{
	pthread_mutex_lock(&shared->fork_mutex[right_fork]);
	shared->fork[right_fork] = 1;
	pthread_mutex_lock(&shared->fork_mutex[left_fork]);
	shared->fork[left_fork] = 1;
	print_status(shared, num, FORK);
}

void	put_down_forks(t_philo *shared, int left_fork, int right_fork)
{
	shared->fork[right_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[left_fork]);
	shared->fork[left_fork] = 0;
	pthread_mutex_unlock(&shared->fork_mutex[right_fork]);
}

int	split_usleep(t_philo *shared, useconds_t ms)
{
	struct timeval	standard;
	struct timeval	passed;
	long	passed_sec;
	long	passed_usec;

	gettimeofday(&standard, NULL);
	passed_sec = 0;
	passed_usec = 0;
	while (passed_sec * 1000000 + passed_usec <= ms)
	{
		if (dead_check(shared) == DEAD)
			return (DEAD);
		gettimeofday(&passed, NULL);
		passed_sec = passed.tv_sec - standard.tv_sec;
		passed_usec = passed.tv_usec - standard.tv_usec;
		usleep(200);
	}
	return (0);
}

int	eating_spagetti(t_philo *shared, int num, int left_fork, int right_fork)
{
	pick_up_forks(shared, num, left_fork, right_fork);
	print_status(shared, num, EAT);
	if (split_usleep(shared, shared->conditions->time_to_eat * 1000) == DEAD) // usleep 하다가 죽으면 return DEAD
	{
		put_down_forks(shared, left_fork, right_fork);	
		return (DEAD);
	}
	put_down_forks(shared, left_fork, right_fork);
	return (0);
}

int	sleeping(t_philo *shared, int num)
{
	print_status(shared, num, SLEEP);
	if (split_usleep(shared, shared->conditions->time_to_sleep * 1000) == DEAD)
		return (DEAD);
	return (0);
}

void	*philosopher_do_something(void *philo_shared)
{
	t_philo	*shared;
	int		num;
	int		left_fork;
	int		right_fork;

	shared = (t_philo *)philo_shared;

	pthread_mutex_lock(&shared->mutexes[INDEX_M]);
	shared->index++;
	num = shared->index;
	pthread_mutex_unlock(&shared->mutexes[INDEX_M]);

	right_fork = num - 1;
	if (num == 1)
		left_fork = shared->conditions->philo_number - 1;
	else
		left_fork = num - 2;

	while (1)
	{
		if (eating_spagetti(shared, num, left_fork, right_fork) == DEAD)
			break ;
	 	if (sleeping(shared, num) == DEAD)
		 	break ;
		print_status(shared, num, THINK);
	}
	return (NULL);
}

int survive_check(t_philo *shared)
{
	unsigned int	i;
	unsigned int	die_time;
	unsigned int	num;
	
	die_time = shared->conditions->time_to_die;
	num = shared->conditions->philo_number;
	gettimeofday(&shared->start_point, NULL);
	i = 0;
	while (1)
	{
		configure_time_stamp(shared);
		pthread_mutex_lock(&shared->mutexes[DIE_M]);
		if (shared->time_stamp - shared->last_eat[i % num] > die_time)
		{
			printf("%d 왜죽음? lasteat %ld / %ld %u\n",i % num + 1, shared->last_eat[i % num], shared->time_stamp - shared->last_eat[i % num] , die_time);
			print_status(shared, i % num + 1, DEAD);
			shared->die_flags = DEAD;
			break ;
		}
		pthread_mutex_unlock(&shared->mutexes[DIE_M]);
		i++;
		usleep(100);
	}
	usleep(10 * 1000);
	return (0);
}

int	generate_philo(t_conditions *conditions, t_philo *shared)
{
	int				i;

    i = 0;

    shared->conditions = conditions;
	i = 0;
    while (i < conditions->philo_number)
	{
        pthread_create(&shared->philos[i], NULL, philosopher_do_something, shared);
		pthread_detach(shared->philos[i]);
        i++;
    }
	//pthread_mutex_lock(&shared->mutexes[WAIT_M]);
    survive_check(shared);
    return (0);
}


int	main(int argc, char *argv[])
{
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
    init_shared_mem(&philo_share, &conditions);
	generate_philo(&conditions, &philo_share);
	return (0);
}
