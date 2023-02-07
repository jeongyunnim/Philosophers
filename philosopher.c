/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 17:47:09 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	end_check(t_philo *shared)
{
	char flag;
	
	pthread_mutex_lock(&shared->mutexes[END_M]);
	flag = shared->end_flag;
	pthread_mutex_unlock(&shared->mutexes[END_M]);
	return (flag);
}

int	print_status(t_philo *shared, int num, char status)
{
	pthread_mutex_lock(&shared->mutexes[PRT_M]);
	if (end_check(shared) == END)
	{
		pthread_mutex_unlock(&shared->mutexes[PRT_M]);
		return (END);
	}
	if (status == EAT)
		printf("%ld %d is eating\n", get_time() - shared->start_point, num);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", get_time() - shared->start_point, num);
	else if (status == THINK)
		printf("%ld %d is thinking\n", get_time() - shared->start_point, num);
	else if (status == DEAD)
	{
		printf("%ld %d died\n", get_time() - shared->start_point, num);
		return (0);
	}
	else if (status == FORK)
		printf("%ld %d has taken a fork\n", get_time() - shared->start_point, num);
	pthread_mutex_unlock(&shared->mutexes[PRT_M]);
	return (0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	split_usleep(useconds_t ms)
{
	long	standard;

	standard = get_time();
	while (get_time() - standard <= ms)
	{
		usleep(256);
	}
}

int	generate_philo(t_philo *shared)
{
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	shared->start_point = get_time();
	while (i < shared->conditions->philo_number)
	{
		flag = pthread_create(&shared->philos[i], NULL, philosopher_do_something, shared);
		if (flag != 0)
			return (ERROR);
		i++;
	}
	return (0);
}

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
	destroy_mutex_array(shared->last_eat_mutex, num);
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
