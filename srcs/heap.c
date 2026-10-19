/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:00:00 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	sift_up(t_coder **h, int i)
{
	int	p;

	while (i > 0)
	{
		p = (i - 1) / 2;
		if (!heap_before(h[i], h[p]))
			return ;
		heap_swap(&h[i], &h[p]);
		i = p;
	}
}

static void	sift_down(t_coder **h, int size, int i)
{
	int	m;
	int	l;

	while (1)
	{
		l = 2 * i + 1;
		m = i;
		if (l < size && heap_before(h[l], h[m]))
			m = l;
		if (l + 1 < size && heap_before(h[l + 1], h[m]))
			m = l + 1;
		if (m == i)
			return ;
		heap_swap(&h[i], &h[m]);
		i = m;
	}
}

void	heap_push(t_dongles *d, t_coder *c)
{
	d->heap[d->heap_size] = c;
	sift_up(d->heap, d->heap_size);
	d->heap_size++;
}

void	heap_fix(t_dongles *d, int i)
{
	sift_up(d->heap, i);
	sift_down(d->heap, d->heap_size, i);
}
