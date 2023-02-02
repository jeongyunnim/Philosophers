/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:52:27 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/02 19:30:16 by jeseo            ###   ########.fr       */
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
# define DEAD 1

# define INDEX_M 0
# define WAIT_M 1
# define DIE_M 2

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
	pthread_mutex_t		*fork_mutex;
	pthread_mutex_t		mutexes[3];
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

#endif