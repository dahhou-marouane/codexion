/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 10:53:28 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:48:45 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_lock_dongles(t_coder *coder)
{
	if (coder->left->id < coder->right->id)
	{
		pthread_mutex_lock(coder->left->lock);
		pthread_mutex_lock(coder->right->lock);
	}
	else
	{
		pthread_mutex_lock(coder->right->lock);
		pthread_mutex_lock(coder->left->lock);
	}
}

void	ft_unlock_dongles(t_coder *coder)
{
	pthread_mutex_unlock(coder->right->lock);
	pthread_mutex_unlock(coder->left->lock);
}

void	ft_release_dongles(t_coder *coder)
{
	long	now;

	now = ft_get_curr_t_mic();
	ft_lock_dongles(coder);
	coder->left->taken = 0;
	coder->right->taken = 0;
	coder->left->cooldown_end = coder->codexion->args->dongle_cooldown + now;
	coder->right->cooldown_end = coder->codexion->args->dongle_cooldown + now;
	ft_unlock_dongles(coder);
}
