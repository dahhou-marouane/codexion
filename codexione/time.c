/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:20:38 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:44:03 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_usleep(long time, t_codexion *codexion)
{
	long	start;
	long	remaining;

	start = ft_get_curr_t_mic() + time;
	while (!ft_sim_is_stoped(codexion))
	{
		remaining = start - ft_get_curr_t_mic();
		if (remaining <= 0)
			break ;
		if (remaining > 1000)
			usleep(1000);
		else
			usleep(remaining);
	}
}

long	ft_get_curr_t_mic(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}
