/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:52:27 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/27 14:09:40 by jeseo            ###   ########.fr       */
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
# define DEAD -1

typedef struct s_philo_conditions
{
	int		philo_number;
	int		time_to_die;
	int		time_to_sleep;
	int		time_to_eat;
	int		must_eat;
}			t_philo_conditions;

typedef struct  s_philo
{
	t_philo_conditions	*conditions;
    pthread_mutex_t		*fork;
	pthread_mutex_t		tv_mutex;
    struct timeval      tv;
    struct timeval      start_point;
	int					index;
	long				*last_eat;
}               t_lock; // philo로 바꿔줘야겠어.

int		ft_atoi(const char *str);
int		ft_isnum(char c);
int		argument_num_check(char *argv[]);
int		init_conditions(char *argv[], t_philo_conditions *conditions);
int		parse_arguments(char *argv[], t_philo_conditions *conditions);

#endif