/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:04:04 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 16:04:13 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	return_error(t_philo *shared)
{
	pthread_mutex_lock(&shared->mutexes[END_M]);
	shared->end_flag = END;
	pthread_mutex_unlock(&shared->mutexes[END_M]);
	// wait_philos(shared);
	free_structure(shared);
	write(2, "Error\nPthread_create error\n", 27);
	return (ERROR);
}
