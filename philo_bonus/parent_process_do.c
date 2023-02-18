/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process_do.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:23:27 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/18 22:47:29 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	parent_process_do(t_philo *shared, pid_t *childs)
{
	philo_wait(shared, childs);
	free(childs);
	free_structure(shared);
}
