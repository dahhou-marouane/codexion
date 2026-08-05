/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:26:21 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/06 00:13:28 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_print_logs(t_codexion *codexion, int id, int msg)
{
	const char	*msgs[5];
	long		now;

	msgs[0] = "has taken a dongle";
	msgs[1] = "is compiling";
	msgs[2] = "is debugging";
	msgs[3] = "is refactoring";
	msgs[4] = "burned out";
	pthread_mutex_lock(codexion->log_lock);
	if (!ft_sim_is_stoped(codexion) || msg == 4)
	{
		now = ft_get_curr_t_mic();
		if (msg == 0)
			printf("%ld %d %s\n%ld %d %s\n", (now - codexion->start_time_mic)
			/ 1000, id, msgs[0], (now - codexion->start_time_mic)
			/ 1000, id, msgs[0]);
		else
			printf("%ld %d %s\n", (ft_get_curr_t_mic() - codexion->start_time_mic)
				/ 1000, id, msgs[msg]);
	}
	pthread_mutex_unlock(codexion->log_lock);
}

void	ft_print_err(char *msg)
{
	fprintf(stderr, "ERROR: %s\n", msg);
}
