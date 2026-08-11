/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 05:41:29 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:47:53 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_compare(t_heap *heap, int child, int parent)
{
	if (heap->items[child].key == heap->items[parent].key)
		return (heap->items[child].req_time < heap->items[parent].req_time);
	return (heap->items[child].key < heap->items[parent].key);
}

int	peek(t_heap *heap)
{
	if (heap->size == 0)
		return (-1);
	return (heap->items[0].coder_id);
}
