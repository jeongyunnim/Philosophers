/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:52:27 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/03 20:06:45 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>

# define ERROR -1

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK 4
# define DEAD 5

# define TOTAL_MUTEX 5

# define INDEX_M 0
# define WAIT_M 1
# define DIE_M 2
# define LASTEAT_M 3
# define TIME_M 4


typedef struct s_philo_conditions
{
	int		philo_number;
	int		time_to_die;
	int		time_to_sleep;
	int		time_to_eat;
	int		must_eat;
}			t_conditions;

typedef struct  s_philo
{
	struct timeval      tv;
	struct timeval      start_point;
	t_conditions		*conditions;
	pthread_t			*philos;
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		*mutexes;
	int					*fork;
	long				*last_eat;
	long				time_stamp;
	int					index;
	char				die_flags;
}               t_philo;

int		ft_atoi(const char *str);
int		ft_isnum(char c);
int		argument_num_check(char *argv[]);
int		init_conditions(char *argv[], t_conditions *conditions);
int		parse_arguments(char *argv[], t_conditions *conditions);

pthread_mutex_t	*init_mutex_array(int num);
void    init_shared_mem(t_philo *philo_shared, t_conditions *conditions);

#endif