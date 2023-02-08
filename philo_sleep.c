/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:04 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 13:22:05 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	sleeping(t_philo *shared, int num)
{
	if (print_status(shared, num, SLEEP) == END)
		return (END);
	split_usleep(shared->conditions->time_to_sleep);
	return (0);
}