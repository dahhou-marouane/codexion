/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:20:38 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/04 06:00:54 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_usleep(long time, t_codexion *codexion)
{
	long			start;
	// long			remaining;

	start = ft_get_curr_t_mic() + time;

	while (!ft_sim_is_stoped(codexion))
	{
		// remaining = time - (ft_get_curr_t_mic() - start);
		// if (remaining <= 0)
		// 	return ;
		// if (remaining > 500)
		// 	usleep(500);
		// else
		// 	usleep(remaining );
		if (ft_get_curr_t_mic() < start)
			usleep(500);
		else
			break;
	}
}

long	ft_get_curr_t_mic(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}
