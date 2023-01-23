/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:52:27 by jeseo             #+#    #+#             */
/*   Updated: 2023/01/23 20:52:32 by jeseo            ###   ########.fr       */
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

typedef struct s_philo_conditions
{
	int		philo_number;
	int		time_to_die;
	int		time_to_sleep;
	int		time_to_eat;
	int		must_eat;
}			t_philo_conditions;

typedef struct  s_lock
{
	t_philo_conditions	*conditions;
    pthread_mutex_t		*fork;
}               t_lock;

int		ft_atoi(const char *str);

#endif