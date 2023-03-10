/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 20:52:27 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:48:24 by jeseo            ###   ########.fr       */
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
# include <semaphore.h>
# include <signal.h>

# define ERROR -1
# define SEM_NAME "/forks"
# define EVT_NAME "/event"
# define PRT_NAME "/print"

typedef enum e_stat
{
	EAT = 1,
	SLEEP,
	THINK,
	FORK,
	DEAD,
	END
}		t_stat;

typedef enum e_mutexes
{
	INDEX_M,
	MNT_M,
	PRT_M,
	END_M,
	TOTAL_MUTEX
}		t_mutexes;

typedef struct s_philo_conditions
{
	int		philo_number;
	int		time_to_die;
	int		time_to_sleep;
	int		time_to_eat;
	int		must_eat;
}			t_conditions;

typedef struct s_philo
{
	t_conditions		*conditions;
	sem_t				*forks;
	sem_t				*event_mutex;
	sem_t				*print_mutex;
	long				last_eat;
	long				start;
	int					index;
	int					eat_cnt;
	char				end_flag;
}				t_philo;

int		ft_atoi(const char *str);
int		ft_isnum(char c);
void	*ft_calloc(size_t n, size_t size);

int		argument_num_check(char *argv[]);
int		init_conditions(char *argv[], t_conditions *conditions);
int		parse_arguments(char *argv[], t_conditions *conditions);

/* child_process_do */
void	child_process_do(t_philo *shared);
void	*philo_do(t_philo *shared);
int		print_status(t_philo *shared, int num, char status);

/* time_utils */
long	get_time(void);
void	split_usleep(useconds_t ms);

/* init_structure */
int		init_shared_mem(t_philo *philo_shared, t_conditions *cndtion);
void	free_structure(t_philo *shared);

/* monitor_philo */
void	*philo_monitoring(void *philo_shared);
void	generate_thread(t_philo *shared);

/* philo_create */
pid_t	*generate_philo(t_philo *shared);
void	philo_wait(t_philo *shared, pid_t *childs);

/* philo eat */
void	put_down_forks(t_philo *shared);
void	eating_spaghetti(t_philo *shared, int num);

/* philo_sleep */
void	sleeping(t_philo *shared, int num);

/* philo_think */
void	thinking(t_philo *shared, int num);

/* error_handle */
int		fork_error(t_philo *shared, pid_t *pid);

/* parent_process_do */
void	parent_process_do(t_philo *shared, pid_t *childs);
int		init_error(void);

#endif
