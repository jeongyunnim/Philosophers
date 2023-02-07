/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_think.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:27:23 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/07 16:58:46 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

//int	thinking(t_philo *shared, int num)
//{
//	if (end_check(shared) != 0)
//		return (END);
//	print_status(shared, num, THINK);
//	return (0);
//}

int	thinking(t_philo *shared, int num)
{
	return (print_status(shared, num, THINK));
}
