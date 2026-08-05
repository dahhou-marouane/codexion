/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 18:20:28 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/02 06:53:41 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_destroy_mutexes(t_mutexes_manager *mutexes)
{
	t_mutexes_manager	*tmp;

	while (mutexes)
	{
		tmp = mutexes->next;
		if (pthread_mutex_destroy(&mutexes->lock) != 0)
		{
			ft_print_err("pthread_mutex_destroy");
			return ;
		}
		free(mutexes);
		mutexes = tmp;
	}
}

static void	ft_destroy_cond(t_cond_manager *conds)
{
	t_cond_manager	*tmp;

	while (conds)
	{
		tmp = conds->next;
		if (pthread_cond_destroy(&conds->cond) != 0)
		{
			ft_print_err("pthread_cond_destroy");
			return ;
		}
		free(conds);
		conds = tmp;
	}
}

static void	ft_join_threads(t_thread_manager *threads)
{
	t_thread_manager	*tmp;

	while (threads)
	{
		tmp = threads->next;
		if (pthread_join(threads->thread, NULL) != 0)
		{
			ft_print_err("pthread_join");
			return ;
		}
		free(threads);
		threads = tmp;
	}
}

void	ft_free_all(t_ressources *ressources)
{
	if (!ressources)
		return ;
	ft_join_threads(ressources->threads);
	ft_destroy_cond(ressources->conds);
	ft_destroy_mutexes(ressources->mutexes);
	ft_free_garbage(ressources->allocs);
	ressources->threads = NULL;
	ressources->conds = NULL;
	ressources->mutexes = NULL;
	ressources->allocs = NULL;
}
