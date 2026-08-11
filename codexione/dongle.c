/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 04:55:29 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:49:01 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	could_take_dongles(t_coder *coder)
{
	long	now;

	now = ft_get_curr_t_mic();
	if (!coder->left->taken && !coder->right->taken
		&& now >= coder->left->cooldown_end && now >= coder->right->cooldown_end
		&& peek(&coder->left->heap) == coder->id
		&& peek(&coder->right->heap) == coder->id)
		return (1);
	return (0);
}

static void	take_dongles(t_coder *coder)
{
	coder->left->taken = 1;
	coder->right->taken = 1;
	heap_remove_id(&coder->left->heap, coder->id);
	heap_remove_id(&coder->right->heap, coder->id);
	ft_unlock_dongles(coder);
	ft_print_logs(coder->codexion, coder->id, 0);
	ft_compile(coder);
}

static int	ft_reaquire_dongles(t_coder *coder, t_request *req)
{
	int	is_left_peek;
	int	is_right_peek;

	is_left_peek = (peek(&coder->left->heap) == coder->id);
	is_right_peek = (peek(&coder->right->heap) == coder->id);
	if (is_left_peek != is_right_peek)
	{
		heap_remove_id(&coder->left->heap, coder->id);
		heap_remove_id(&coder->right->heap, coder->id);
		ft_unlock_dongles(coder);
		ft_usleep(300, coder->codexion);
		ft_lock_dongles(coder);
		push(&coder->left->heap, req);
		push(&coder->right->heap, req);
		return (1);
	}
	return (0);
}

int	ft_aquire_dongles(t_coder *coder)
{
	t_request	req;

	req.coder_id = coder->id;
	req.key = ft_get_key(coder);
	req.req_time = ft_get_curr_t_mic();
	ft_lock_dongles(coder);
	push(&coder->left->heap, &req);
	push(&coder->right->heap, &req);
	while (!ft_sim_is_stoped(coder->codexion))
	{
		if (could_take_dongles(coder))
		{
			take_dongles(coder);
			return (1);
		}
		if (ft_reaquire_dongles(coder, &req))
			continue ;
		ft_unlock_dongles(coder);
		ft_usleep(300, coder->codexion);
		ft_lock_dongles(coder);
	}
	heap_remove_id(&coder->left->heap, coder->id);
	heap_remove_id(&coder->right->heap, coder->id);
	ft_unlock_dongles(coder);
	return (0);
}
