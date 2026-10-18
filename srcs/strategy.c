/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:25:55 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:27:24 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	before(t_coder *a, t_coder *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	return (a->ticket < b->ticket);
}

static int	ready(t_dongles *d, t_coder *q)
{
	t_dongles	*other;

	other = q->left;
	if (q->left == d)
		other = q->right;
	return (!other->is_taken);
}

static int	is_first(t_dongles *d, t_coder *c)
{
	int	i;

	i = 0;
	while (i < d->queue_size)
	{
		if (d->queue[i] != c && before(d->queue[i], c)
			&& ready(d, d->queue[i]))
			return (0);
		i++;
	}
	return (1);
}

int	can_take(t_coder *c)
{
	long	now;

	now = get_time();
	return (!c->left->is_taken && !c->right->is_taken
		&& now >= c->left->availabilitytime
		&& now >= c->right->availabilitytime
		&& is_first(c->left, c) && is_first(c->right, c));
}

void	release_dongles(t_coder *c)
{
	t_data	*d;
	long	until;

	d = c->data;
	until = get_time() + d->cooldown;
	pthread_mutex_lock(&d->sched_mutex);
	c->left->is_taken = 0;
	c->right->is_taken = 0;
	c->left->availabilitytime = until;
	c->right->availabilitytime = until;
	pthread_cond_broadcast(&d->sched_cond);
	pthread_mutex_unlock(&d->sched_mutex);
}
