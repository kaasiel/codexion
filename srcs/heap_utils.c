/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:00:00 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_before(t_coder *a, t_coder *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	return (a->ticket < b->ticket);
}

void	heap_swap(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_coder	*heap_top(t_dongles *d)
{
	if (d->heap_size == 0)
		return (NULL);
	return (d->heap[0]);
}

void	heap_remove(t_dongles *d, t_coder *c)
{
	int	i;

	i = 0;
	while (i < d->heap_size && d->heap[i] != c)
		i++;
	if (i == d->heap_size)
		return ;
	d->heap_size--;
	if (i == d->heap_size)
		return ;
	d->heap[i] = d->heap[d->heap_size];
	heap_fix(d, i);
}
