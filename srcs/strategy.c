/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:00:00 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	t_coder	*top;

	top = heap_top(d);
	return (top == c || !ready(d, top));
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

void	set_priority(t_coder *c)
{
	t_data	*d;
	long	deadline;

	d = c->data;
	pthread_mutex_lock(&c->mutex);
	deadline = c->last_compile_start + c->time_to_burntout;
	pthread_mutex_unlock(&c->mutex);
	pthread_mutex_lock(&d->ticket_mutex);
	c->ticket = d->ticket++;
	pthread_mutex_unlock(&d->ticket_mutex);
	c->key = c->ticket;
	if (d->strategy == AL_EDF)
		c->key = deadline;
}

void	release_dongles(t_coder *c)
{
	t_dongles	*first;
	t_dongles	*second;
	long		until;

	first = c->left;
	second = c->right;
	if (first->id > second->id)
	{
		first = c->right;
		second = c->left;
	}
	until = get_time() + c->data->cooldown;
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	first->is_taken = 0;
	second->is_taken = 0;
	first->availabilitytime = until;
	second->availabilitytime = until;
	pthread_cond_broadcast(&first->cond);
	pthread_cond_broadcast(&second->cond);
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}
