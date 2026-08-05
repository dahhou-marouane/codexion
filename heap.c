/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahhou <mdahhou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 13:26:18 by mdahhou           #+#    #+#             */
/*   Updated: 2026/08/04 03:07:11 by mdahhou          ###   ########.fr       */
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

static void	sift_down(t_heap *heap)
{
	int	i;
	int	left;
	int	right;
	int	smallest;

	i = 0;
	while (1)
	{
		left = (i * 2) + 1;
		right = (i * 2) + 2;
		smallest = i;
		if (left < heap->size && (heap->items[left].key
				< heap->items[smallest].key || (heap->items[left].key
				== heap->items[smallest].key && heap->items[left].req_time
				< heap->items[smallest].req_time)))
			smallest = left;
		if (right < heap->size && (heap->items[right].key
				< heap->items[smallest].key || (heap->items[right].key
				== heap->items[smallest].key && heap->items[right].req_time
				< heap->items[smallest].req_time)))
			smallest = right;
		if (smallest == i)
			break ;
		swap(&heap->items[i], &heap->items[smallest]);
		i = smallest;
	}
}

int	heap_compare(t_heap *heap, int child, int parent)
{
	if (heap->items[child].key == heap->items[parent].key)
		return (heap->items[child].req_time < heap->items[parent].req_time);
	return (heap->items[child].key < heap->items[parent].req_time);
}

static void	sift_up(t_heap *heap, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->items[i].key < heap->items[parent].key)
		{
			swap(&heap->items[i], &heap->items[parent]);
			i = parent;
		}
		else if (heap->items[i].key == heap->items[parent].key
			&& heap->items[i].req_time < heap->items[parent].req_time)
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

void	pop(t_heap *heap)
{
	heap->size--;
	heap->items[0] = heap->items[heap->size];
	sift_down(heap);
}

int	peek(t_heap *heap)
{
	return (heap->items[0].coder_id);
}
