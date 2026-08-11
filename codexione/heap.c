/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:26:18 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/07 16:48:07 by mdahhou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_down(t_heap *heap, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = (i * 2) + 1;
		right = (i * 2) + 2;
		smallest = i;
		if (left < heap->size && heap_compare(heap, left, smallest))
			smallest = left;
		if (right < heap->size && heap_compare(heap, right, smallest))
			smallest = right;
		if (smallest == i)
			break ;
		swap(&heap->items[i], &heap->items[smallest]);
		i = smallest;
	}
}

static void	sift_up(t_heap *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap_compare(heap, i, parent))
		{
			swap(&heap->items[i], &heap->items[parent]);
			i = parent;
		}
		else
			break ;
	}
}

void	push(t_heap *heap, t_request *req)
{
	heap->items[heap->size] = *req;
	sift_up(heap, heap->size);
	heap->size++;
}

void	heap_remove_id(t_heap *heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->items[i].coder_id == coder_id)
		{
			heap->size--;
			if (i == heap->size)
				return ;
			heap->items[i] = heap->items[heap->size];
			sift_down(heap, i);
			sift_up(heap, i);
			return ;
		}
		i++;
	}
}
