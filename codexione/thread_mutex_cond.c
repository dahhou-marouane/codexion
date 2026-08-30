/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_mutex_cond.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:46:38 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/25 04:47:24 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

pthread_mutex_t	*ft_mutex_create(t_ressources *ressources)
{
	t_mutexes_manager	*n_node;

	if (!ressources)
		return (NULL);
	n_node = ft_malloc(sizeof(t_mutexes_manager), ressources);
	if (!n_node)
	{
		ft_print_err("Malloc failed.");
		ft_free_all(ressources);
		return (NULL);
	}
	if (pthread_mutex_init(&n_node->lock, NULL) != 0)
	{
		ft_print_err("Pthread_mutex_init failed.");
		ft_free_all(ressources);
		return (NULL);
	}
	n_node->next = ressources->mutexes;
	ressources->mutexes = n_node;
	return (&n_node->lock);
}

pthread_cond_t	*ft_cond_create(t_ressources *ressources)
{
	t_cond_manager	*n_node;

	if (!ressources)
		return (NULL);
	n_node = ft_malloc(sizeof(t_cond_manager), ressources);
	if (!n_node)
	{
		ft_print_err("Malloc failed.");
		ft_free_all(ressources);
		return (NULL);
	}
	if (pthread_cond_init(&n_node->cond, NULL) != 0)
	{
		ft_print_err("Pthread_cond_init failed.");
		ft_free_all(ressources);
		return (NULL);
	}
	n_node->next = ressources->conds;
	ressources->conds = n_node;
	return (&n_node->cond);
}

pthread_t	*ft_pthread_create(t_ressources *ressources, void *(*f)(void *),
		void *arg)
{
	t_thread_manager	*n_node;

	if (!ressources)
		return (NULL);
	n_node = ft_malloc(sizeof(t_thread_manager), ressources);
	if (!n_node)
	{
		ft_print_err("Malloc failed.");
		return (NULL);
	}
	if (pthread_create(&n_node->thread, NULL, f, arg) != 0)
	{
		ft_print_err("Pthread_create failed.");
		return (NULL);
	}
	n_node->next = ressources->threads;
	ressources->threads = n_node;
	return (&n_node->thread);
}
