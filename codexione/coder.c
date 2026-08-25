/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:28:59 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/25 04:21:02 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	ft_get_key(t_coder *coder)
{
	long	key;

	pthread_mutex_lock(coder->codexion->seq_lock);
	key = ((coder->codexion->args->fifo) * coder->codexion->seq++)
		+ ((!coder->codexion->args->fifo) * (coder->l_com_start
				+ coder->codexion->args->t_to_burnout));
	pthread_mutex_unlock(coder->codexion->seq_lock);
	return (key);
}

void	ft_compile(t_coder *coder)
{
	pthread_mutex_lock(coder->lock);
	coder->l_com_start = ft_get_curr_t_mic();
	coder->compile_count++;
	pthread_mutex_unlock(coder->lock);
	ft_usleep(coder->codexion->args->t_to_compile, coder->codexion);
	if (coder->compile_count == coder->codexion->args->nb_of_compiles_req)
	{
		pthread_mutex_lock(coder->codexion->f_routine_lock);
		coder->codexion->f_routine++;
		pthread_mutex_unlock(coder->codexion->f_routine_lock);
	}
}

static void	ft_debug(t_coder *coder)
{
	ft_print_logs(coder->codexion, coder->id, 2);
	ft_usleep(coder->codexion->args->t_to_debug, coder->codexion);
}

static void	ft_refactor(t_coder *coder)
{
	ft_print_logs(coder->codexion, coder->id, 3);
	ft_usleep(coder->codexion->args->t_to_refactor, coder->codexion);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (!single_coder(coder))
		return (NULL);
	if (coder->id % 2 == 0)
		ft_usleep(coder->codexion->args->t_to_compile
			+ coder->codexion->args->dongle_cooldown, coder->codexion);
	else if (coder->id == coder->codexion->args->nb_of_coders
		&& coder->codexion->args->nb_of_coders % 2 == 1)
		ft_usleep(2 * (coder->codexion->args->t_to_compile
				+ coder->codexion->args->dongle_cooldown), coder->codexion);
	while (!ft_sim_is_stoped(coder->codexion))
	{
		if (!ft_aquire_dongles(coder))
			return (NULL);
		ft_release_dongles(coder);
		ft_debug(coder);
		ft_refactor(coder);
	}
	return (NULL);
}
