/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:04 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 21:20:39 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

//int	sleeping(t_philo *shared, int num)
//{
//	if (end_check(shared) != 0)
//		return (END);
//	print_status(shared, num, SLEEP);
//	split_usleep(shared->conditions->time_to_sleep);
//	return (0);
//}

int	sleeping(t_philo *shared, int num)
{
	if (print_status(shared, num, SLEEP) == END)
		return (END);
	split_usleep(shared->conditions->time_to_sleep);
	return (0);
}