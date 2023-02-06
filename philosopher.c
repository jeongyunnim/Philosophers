/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/06 17:13:37 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	end_check(t_philo *shared)
{
	char flag;
	
	pthread_mutex_lock(&shared->mutexes[END_M]);
	flag = shared->end_flag;
	pthread_mutex_unlock(&shared->mutexes[END_M]);
	if (flag == END)
	{
		return (END);
	}
	return (0);
}

void	configure_time_stamp(t_philo *shared)
{
	long	passed_sec;
	long	passed_usec;

	pthread_mutex_lock(&shared->mutexes[TIME_M]);
	gettimeofday(&shared->tv, NULL);
	passed_sec = shared->tv.tv_sec - shared->start_point.tv_sec;
	passed_usec = shared->tv.tv_usec - shared->start_point.tv_usec;
	shared->time_stamp = passed_sec * 1000 + passed_usec / 1000;
	pthread_mutex_unlock(&shared->mutexes[TIME_M]);
}

int	print_status(t_philo *shared, int num, char status)
{
	pthread_mutex_lock(&shared->mutexes[TIME_M]);
	if (status == EAT)
		printf("%ld %d is eating\n", shared->time_stamp, num);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", shared->time_stamp, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", shared->time_stamp, num);
	else if (status == DEAD)
		printf("%ld %d died\n", shared->time_stamp, num);
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", shared->time_stamp, num);
	else // 이건 지워라
		printf("%ld %d has put a fork\n", shared->time_stamp, num);
	pthread_mutex_unlock(&shared->mutexes[TIME_M]);
	return (0);
}

void	split_usleep(t_philo *shared, useconds_t ms)
{
	long	standard_sec;
	long	standard_usec;
	long	passed_time;

	ms *= 1000;
	passed_time = 0;
	standard_sec = shared->tv.tv_sec;
	standard_usec = shared->tv.tv_usec;
	while (passed_time <= ms)
	{
		configure_time_stamp(shared);
		passed_time = (shared->tv.tv_sec - standard_sec) * 1000000 \
		+ shared->tv.tv_usec - standard_usec;
		usleep(256);
	}
}

int	generate_philo(t_philo *shared)
{
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	gettimeofday(&shared->start_point, NULL);

	while (i < shared->conditions->philo_number)
	{
		flag = pthread_create(&shared->philos[i], NULL, philosopher_do_something, shared);
		if (flag != 0)
			return (ERROR);
		i++;
	}

	configure_time_stamp(shared);

	pthread_mutex_lock(&shared->mutexes[WAIT_M]);
	return (0);
}

//int monitor(t_philo *shared)
//{
//	pthread_t	monitoring;

//	pthread_create(&monitoring, NULL, thread_monitoring, shared);
//	pthread_join(monitoring, NULL);
//	return (0);
//}

void	wait_philos(t_philo *shared)
{
	int	num;
	int	i;

	num = shared->conditions->philo_number;
	i = 0;
	while (i < num)
	{
		pthread_join(shared->philos[i], NULL);
		i++;
	}
}

void	free_structure(t_philo *shared)
{
	int num;

	num = shared->conditions->philo_number;
	destroy_mutex_array(shared->fork_mutex, num);
	destroy_mutex_array(shared->mutexes, TOTAL_MUTEX);
	free(shared->philos);
	free(shared->last_eat);
	free(shared->fork);
	free(shared->eat_cnt);
	memset(shared, 0, sizeof(*shared));
}

int	main(int argc, char *argv[])
{
	t_philo             philo_share;
	t_conditions  		conditions;

	if (argc != 5 && argc != 6)
	{
		write(2, "Error\nArgument count error\n", 27);
		return (ERROR);
	}
	if (parse_arguments(argv, &conditions) != 0)
	{
		return (ERROR);
	}
	init_shared_mem(&philo_share, &conditions);
	if (generate_philo(&philo_share) == ERROR)
	{
		pthread_mutex_lock(&philo_share.mutexes[END_M]);
		philo_share.end_flag = END;
		pthread_mutex_unlock(&philo_share.mutexes[END_M]);
		wait_philos(&philo_share);
		free_structure(&philo_share);
		write(2, "Error\nPthread_create error\n", 27);
		return (ERROR);
	}
	thread_monitoring(&philo_share);
	wait_philos(&philo_share);
	free_structure(&philo_share);
	return (0);
}
