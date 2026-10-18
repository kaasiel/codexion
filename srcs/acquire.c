/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acquire.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 07:26:05 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/18 07:26:05 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	queue_dongles(t_dongles *d, t_coder *c, int add)
{
	int	i;
	int	j;

	if (add)
		d->queue[d->queue_size++] = c;
	else
	{
		i = 0;
		j = 0;
		while (i < d->queue_size)
		{
			if (d->queue[i] != c)
				d->queue[j++] = d->queue[i];
			i++;
		}
		d->queue_size = j;
	}
}

int	sim_end(t_data *d)
{
	int	stop;

	pthread_mutex_lock(&d->print_mutex);
	stop = d->simulation_end;
	pthread_mutex_unlock(&d->print_mutex);
	return (stop);
}

void	wait_ms(t_data *d, long ms)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + ms / 1000;
	ts.tv_nsec = (tv.tv_usec + (ms % 1000) * 1000) * 1000;
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	pthread_cond_timedwait(&d->sched_cond, &d->sched_mutex, &ts);
}

static void	prepare_acquire(t_coder *c, t_data *d, long deadline)
{
	c->ticket = d->ticket++;
	c->key = c->ticket;
	if (d->strategy == AL_EDF)
		c->key = deadline;
	queue_dongles(c->left, c, 1);
	queue_dongles(c->right, c, 1);
}

int	acquire_dongles(t_coder *c)
{
	t_data	*d;
	long	deadline;

	d = c->data;
	pthread_mutex_lock(&c->mutex);
	deadline = c->last_compile_start + c->time_to_burntout;
	pthread_mutex_unlock(&c->mutex);
	pthread_mutex_lock(&d->sched_mutex);
	prepare_acquire(c, d, deadline);
	while (!can_take(c) && !sim_end(d))
		wait_ms(d, 1);
	queue_dongles(c->left, c, 0);
	queue_dongles(c->right, c, 0);
	if (sim_end(d))
		return (pthread_mutex_unlock(&d->sched_mutex), 0);
	c->left->is_taken = 1;
	c->right->is_taken = 1;
	pthread_mutex_unlock(&d->sched_mutex);
	printlog(c, "\33[34mhas taken a dongle\33[0m");
	printlog(c, "\33[34mhas taken a dongle\33[0m");
	return (1);
}
