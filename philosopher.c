/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:49:33 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/05 20:51:21 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

char	dead_check(t_philo *shared)
{
	pthread_mutex_lock(&shared->mutexes[DIE_M]);
	if (shared->die_flag == DEAD)
	{
		pthread_mutex_unlock(&shared->mutexes[DIE_M]);
		return (DEAD);
	}
	pthread_mutex_unlock(&shared->mutexes[DIE_M]);
	return (0);
}

char	eat_check(t_philo *shared)
{
	pthread_mutex_lock(&shared->mutexes[EATCNT_M]);
	if (shared->eat_flag == EAT)
	{
		pthread_mutex_unlock(&shared->mutexes[EATCNT_M]);
		return (EAT);
	}
	pthread_mutex_unlock(&shared->mutexes[EATCNT_M]);
	return (0);
}

int	check_dead_or_full(t_philo *shared)
{
	if (dead_check(shared) == DEAD || eat_check(shared) == EAT)
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
	pthread_mutex_unlock(&shared->mutexes[TIME_M]);
	return (0);
}

void	split_usleep(useconds_t ms)
{
	struct timeval	standard;
	struct timeval	passed;
	long	passed_sec;
	long	passed_usec;

	gettimeofday(&standard, NULL);
	ms *= 1000;
	passed_sec = 0;
	passed_usec = 0;
	while (passed_sec * (1000000)+ passed_usec <= ms)
	{
		gettimeofday(&passed, NULL);
		passed_sec = passed.tv_sec - standard.tv_sec;
		passed_usec = passed.tv_usec - standard.tv_usec;
		usleep(128);
	}
}

int	sleeping(t_philo *shared, int num)
{
	if (check_dead_or_full(shared) != 0)
		return (END);
	print_status(shared, num, SLEEP);
	split_usleep(shared->conditions->time_to_sleep);
	return (0);
}

int	thinking(t_philo *shared, int num)
{
	if (check_dead_or_full(shared) != 0)
		return (END);
	print_status(shared, num, THINK);
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

	pthread_mutex_lock(&shared->mutexes[WAIT_M]);
	pthread_mutex_unlock(&shared->mutexes[WAIT_M]);
	if (num % 2 == 0)
		usleep(100);
	while (1)
	{
		if (eating_spaghetti(shared, num, left_fork, right_fork) != 0)
			break ;
	 	if (sleeping(shared, num) != 0)
		 	break ;
		if (thinking(shared, num) != 0)
			break ;
	}
	return (NULL);
}

int dead_monitor(t_philo *shared, unsigned int i, int num, int die_time)
{
	pthread_mutex_lock(&shared->mutexes[DIE_M]);
	pthread_mutex_lock(&shared->mutexes[TIME_M]);
	pthread_mutex_lock(&shared->mutexes[LASTEAT_M]);
	if (shared->time_stamp - shared->last_eat[i % num] > die_time)
	{
		pthread_mutex_unlock(&shared->mutexes[LASTEAT_M]);
		pthread_mutex_unlock(&shared->mutexes[TIME_M]);
		print_status(shared, i % num + 1, DEAD);
		shared->die_flag = DEAD;
		//pthread_mutex_unlock(&shared->mutexes[DIE_M]);
		return (DEAD);
	}
	pthread_mutex_unlock(&shared->mutexes[LASTEAT_M]);
	pthread_mutex_unlock(&shared->mutexes[TIME_M]);
	pthread_mutex_unlock(&shared->mutexes[DIE_M]);
	return (0);
}

int	eatcnt_monitor(t_philo *shared, int num)
{
	int		i;
	char	flag;

	i = 0;
	flag = 1;
	if (shared->conditions->must_eat == 0)
		return (0);
	while (i < num)
	{
		if (shared->eat_cnt[i] < shared->conditions->must_eat)
		{
			flag = 0;
			break ;
		}
		i++;
	}
	if (flag == 1)
	{
		pthread_mutex_lock(&shared->mutexes[EAT]);
		shared->eat_flag = 1;
		pthread_mutex_unlock(&shared->mutexes[EAT]);
		return (EAT);
	}
	return (0);
}

void	*thread_monitoring(void *philo_shared)
{
	t_philo			*shared;
	unsigned int	i;
	int				die_time;
	int				num;
	
	shared = (t_philo *)philo_shared;
	die_time = shared->conditions->time_to_die;
	num = shared->conditions->philo_number;
	pthread_mutex_unlock(&shared->mutexes[WAIT_M]);
	i = 0;
	while (1)
	{
		configure_time_stamp(shared);
		if (dead_monitor(shared, i, num, die_time) == DEAD)
			break ;
		if (eatcnt_monitor(shared, num) == EAT)
			break ;
		i++;
		usleep(100);
	}
	sleep(1);
	//printf("monitoring() 종료. 왜 쉬는지는 모르겠군요.\n");
	return (NULL);
}

int	generate_philo(t_conditions *conditions, t_philo *shared)
{
	int				i;

	shared->conditions = conditions;
	i = 0;
	gettimeofday(&shared->start_point, NULL);
	while (i < conditions->philo_number)
	{
		pthread_create(&shared->philos[i], NULL, philosopher_do_something, shared);
		pthread_detach(shared->philos[i]);
		i++;
	}
	pthread_mutex_lock(&shared->mutexes[WAIT_M]);
	return (0);
}

int monitor(t_philo *shared)
{
	pthread_t	monitoring;

	pthread_create(&monitoring, NULL, thread_monitoring, shared);
	pthread_join(monitoring, NULL);
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
	monitor(&philo_share);
	return (0);
}
