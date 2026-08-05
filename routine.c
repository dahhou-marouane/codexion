/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:28:59 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/04 03:56:18 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_compile(t_coder *coder)
{
	pthread_mutex_lock(coder->lock);
	coder->l_com_start = ft_get_curr_t_mic();
	coder->compile_count++;
	pthread_mutex_unlock(coder->lock);
	ft_print_logs(coder->codexion, coder->id, 1);
	ft_usleep(coder->codexion->args->t_to_compile, coder->codexion);
	if (coder->compile_count == coder->codexion->args->nb_of_compiles_req)
	{
		pthread_mutex_lock(coder->codexion->f_routine_lock);
		coder->codexion->f_routine++;
		pthread_mutex_unlock(coder->codexion->f_routine_lock);
	}
}

void	ft_debug(t_coder *coder)
{
	ft_print_logs(coder->codexion, coder->id, 2);
	ft_usleep(coder->codexion->args->t_to_debug, coder->codexion);
}

void	ft_refactor(t_coder *coder)
{
	ft_print_logs(coder->codexion, coder->id, 3);
	ft_usleep(coder->codexion->args->t_to_refactor, coder->codexion);
}
