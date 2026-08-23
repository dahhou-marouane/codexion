/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:26:26 by mdahhou           #+#    #+#             */
/*   Updated: 2026/07/27 18:14:26 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_grbmanager	*ft_create_node(void)
{
	t_grbmanager	*node;

	node = malloc(sizeof(t_grbmanager));
	if (!node)
		return (NULL);
	node->p = NULL;
	node->next = NULL;
	return (node);
}

void	ft_free_garbage(t_grbmanager *grbmanager)
{
	t_grbmanager	*tmp;

	while (grbmanager)
	{
		tmp = (grbmanager)->next;
		free(grbmanager->p);
		free(grbmanager);
		(grbmanager) = tmp;
	}
}

void	*ft_malloc(size_t size, t_ressources *ressources)
{
	void			*p;
	t_grbmanager	*new;

	p = malloc(size);
	new = ft_create_node();
	if (!new || !p)
	{
		ft_print_err("Malloc.");
		free(p);
		free(new);
		ft_free_all(ressources);
		return (NULL);
	}
	new->p = p;
	new->next = ressources->allocs;
	ressources->allocs = new;
	return (p);
}
