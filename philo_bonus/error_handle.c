/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 16:04:04 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/15 14:44:20 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

int	return_error(t_philo *shared)
{
	shared->end_flag = END;
	free_structure(shared);
	write(2, "Error\nPthread_create error\n", 27);
	return (ERROR);
}
