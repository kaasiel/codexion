/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: belaindr <belaindr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/19 10:00:00 by belaindr          #+#    #+#             */
/*   Updated: 2026/10/19 10:00:00 by belaindr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	printlog(t_coder *coder, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&coder->data->print_mutex);
	if (!coder->data->simulation_end)
	{
		timestamp = get_time() - coder->data->start_time;
		printf("%ld %d %s\n", timestamp, coder->id, msg);
	}
	pthread_mutex_unlock(&coder->data->print_mutex);
}

int	sim_end(t_data *d)
{
	int	stop;

	pthread_mutex_lock(&d->print_mutex);
	stop = d->simulation_end;
	pthread_mutex_unlock(&d->print_mutex);
	return (stop);
}

void	wait_dongle(t_dongles *d)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = (tv.tv_usec + 1000) * 1000;
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000L;
	}
	pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
}
