/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 09:31:14 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	try_take(t_coder *c)
{
	if (!can_take(c))
		return (0);
	heap_remove(c->left, c);
	heap_remove(c->right, c);
	c->left->is_taken = 1;
	c->right->is_taken = 1;
	return (1);
}

static t_dongles	*pick_wait(t_coder *c)
{
	if (c->right->is_taken && !c->left->is_taken)
		return (c->right);
	return (c->left);
}

static void	wait_turn(t_coder *c, int *ids, int n)
{
	t_dongles	*d;

	d = pick_wait(c);
	window_unlock_but(c->data, ids, n, d->id);
	wait_dongle(d);
	pthread_mutex_unlock(&d->mutex);
	window_lock(c->data, ids, n);
}

static void	finish(t_coder *c, int *ids, int n, int taken)
{
	if (!taken)
	{
		heap_remove(c->left, c);
		heap_remove(c->right, c);
	}
	window_unlock(c->data, ids, n);
	if (taken)
	{
		printlog(c, "\033[34mhas taken a dongle\033[0m");
		printlog(c, "\033[34mhas taken a dongle\033[0m");
	}
}

int	acquire_dongles(t_coder *c)
{
	int	ids[4];
	int	n;
	int	taken;

	set_priority(c);
	n = window_ids(c, ids);
	window_lock(c->data, ids, n);
	heap_push(c->left, c);
	heap_push(c->right, c);
	taken = try_take(c);
	while (!taken && !sim_end(c->data))
	{
		wait_turn(c, ids, n);
		taken = try_take(c);
	}
	finish(c, ids, n, taken);
	return (taken);
}
