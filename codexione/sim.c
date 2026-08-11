/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:20:45 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:45:18 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_sim_is_stoped(t_codexion *codexion)
{
	int	stoped;

	pthread_mutex_lock(codexion->stop_lock);
	stoped = codexion->stop;
	pthread_mutex_unlock(codexion->stop_lock);
	return (stoped);
}

void	ft_set_sim_stop(t_codexion *codexion)
{
	pthread_mutex_lock(codexion->stop_lock);
	codexion->stop = 1;
	pthread_mutex_unlock(codexion->stop_lock);
}

int	ft_start_coders(t_codexion *codexion)
{
	int	i;

	i = 0;
	while (i < codexion->args->nb_of_coders)
	{
		if (!ft_pthread_create(&codexion->ressources, coder_routine,
				&codexion->coders[i]))
		{
			ft_set_sim_stop(codexion);
			pthread_mutex_lock(codexion->start_lock);
			codexion->start = 1;
			pthread_cond_broadcast(codexion->start_cond);
			pthread_mutex_unlock(codexion->start_lock);
			return (0);
		}
		i++;
	}
	return (1);
}

int	ft_start_monitor(t_codexion *codexion)
{
	if (!ft_pthread_create(&codexion->ressources, monitor_routine, codexion))
	{
		ft_set_sim_stop(codexion);
		pthread_mutex_lock(codexion->start_lock);
		codexion->start = 1;
		pthread_cond_broadcast(codexion->start_cond);
		pthread_mutex_unlock(codexion->start_lock);
		return (0);
	}
	return (1);
}

int	ft_start_sim(t_codexion *codexion)
{
	int	i;

	if (!ft_start_coders(codexion))
		return (0);
	if (!ft_start_monitor(codexion))
		return (0);
	codexion->start_time_mic = ft_get_curr_t_mic();
	i = 0;
	while (i < codexion->args->nb_of_coders)
	{
		codexion->coders[i].l_com_start = codexion->start_time_mic;
		i++;
	}
	pthread_mutex_lock(codexion->start_lock);
	codexion->start = 1;
	pthread_cond_broadcast(codexion->start_cond);
	pthread_mutex_unlock(codexion->start_lock);
	return (1);
}
