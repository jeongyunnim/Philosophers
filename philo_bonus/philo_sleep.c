/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sleep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:28:04 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 21:01:19 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	sleeping(t_philo *shared, int num)
{
	print_status(shared, num, SLEEP);
	split_usleep(shared->conditions->time_to_sleep);
}
