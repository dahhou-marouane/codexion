/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:21:35 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:47:37 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_init_codexion(t_codexion *codexion)
{
	codexion->ressources.allocs = NULL;
	codexion->ressources.threads = NULL;
	codexion->ressources.mutexes = NULL;
	codexion->ressources.conds = NULL;
	codexion->f_routine_lock = ft_mutex_create(&codexion->ressources);
	codexion->seq_lock = ft_mutex_create(&codexion->ressources);
	codexion->log_lock = ft_mutex_create(&codexion->ressources);
	codexion->stop_lock = ft_mutex_create(&codexion->ressources);
	codexion->start_lock = ft_mutex_create(&codexion->ressources);
	codexion->start_cond = ft_cond_create(&codexion->ressources);
	if (!codexion->seq_lock || !codexion->log_lock || !codexion->start_cond
		|| !codexion->stop_lock || !codexion->f_routine_lock
		|| !codexion->start_lock)
		return (0);
	codexion->args = ft_malloc(sizeof(t_arguments), &codexion->ressources);
	if (!codexion->args)
		return (0);
	codexion->dongles = NULL;
	codexion->coders = NULL;
	codexion->stop = 0;
	codexion->seq = 0;
	codexion->start = 0;
	codexion->f_routine = 0;
	return (1);
}

int	ft_init_dongles(t_codexion *codexion)
{
	int	i;

	i = 0;
	codexion->dongles = ft_malloc(sizeof(t_dongle)
			* codexion->args->nb_of_coders, &codexion->ressources);
	if (!codexion->dongles)
		return (0);
	while (i < codexion->args->nb_of_coders)
	{
		codexion->dongles[i].id = i;
		codexion->dongles[i].taken = 0;
		codexion->dongles[i].heap.size = 0;
		codexion->dongles[i].cooldown_end = 0;
		codexion->dongles[i].heap.items = ft_malloc(sizeof(t_request) * 2,
				&codexion->ressources);
		if (!codexion->dongles[i].heap.items)
			return (0);
		codexion->dongles[i].lock = ft_mutex_create(&codexion->ressources);
		if (!codexion->dongles[i].lock)
			return (0);
		i++;
	}
	return (1);
}

int	ft_init_coders(t_codexion *codexion)
{
	int	i;

	i = 0;
	codexion->coders = ft_malloc(sizeof(t_coder) * codexion->args->nb_of_coders,
			&codexion->ressources);
	if (!codexion->coders)
		return (0);
	while (i < codexion->args->nb_of_coders)
	{
		codexion->coders[i].id = i + 1;
		codexion->coders[i].left = &codexion->dongles[i];
		codexion->coders[i].right = &codexion->dongles[(i + 1)
			% codexion->args->nb_of_coders];
		codexion->coders[i].codexion = codexion;
		codexion->coders[i].l_com_start = 0;
		codexion->coders[i].compile_count = 0;
		codexion->coders[i].lock = ft_mutex_create(&codexion->ressources);
		if (!codexion->coders[i].lock)
			return (0);
		i++;
	}
	return (1);
}
