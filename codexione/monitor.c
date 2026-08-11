/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 05:22:25 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:46:57 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_there_burnout(t_codexion *codexion)
{
	long	last;
	int		f_routine;
	int		i;

	i = 0;
	pthread_mutex_lock(codexion->f_routine_lock);
	f_routine = codexion->f_routine;
	pthread_mutex_unlock(codexion->f_routine_lock);
	while (i < codexion->args->nb_of_coders
		&& codexion->args->nb_of_coders != f_routine)
	{
		pthread_mutex_lock(codexion->coders[i].lock);
		last = codexion->coders[i].l_com_start;
		pthread_mutex_unlock(codexion->coders[i].lock);
		if (ft_get_curr_t_mic() - last > codexion->args->t_to_burnout)
		{
			ft_set_sim_stop(codexion);
			ft_print_logs(codexion, i + 1, 4);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_codexion	*codexion;

	codexion = (t_codexion *)arg;
	pthread_mutex_lock(codexion->start_lock);
	while (!codexion->start)
		pthread_cond_wait(codexion->start_cond, codexion->start_lock);
	pthread_mutex_unlock(codexion->start_lock);
	while (!ft_sim_is_stoped(codexion))
	{
		if (is_there_burnout(codexion))
			return (NULL);
		pthread_mutex_lock(codexion->f_routine_lock);
		if (codexion->f_routine == codexion->args->nb_of_coders)
		{
			ft_set_sim_stop(codexion);
			pthread_mutex_unlock(codexion->f_routine_lock);
			return (NULL);
		}
		pthread_mutex_unlock(codexion->f_routine_lock);
		ft_usleep(300, codexion);
	}
	return (NULL);
}
